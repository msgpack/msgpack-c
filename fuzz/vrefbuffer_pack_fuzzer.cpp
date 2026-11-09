// Fuzz harness: encode direction via msgpack::vrefbuffer + round-trip property.
//
// Threat model (why this harness should find bugs):
// - msgpack::pack into a msgpack::vrefbuffer is the zero-copy transmit path
//   (writev gathering): real senders do exactly this on attacker-influenced
//   object graphs. Production coverage never touches it (report 20261108:
//   v1/vrefbuffer.hpp 0/215 lines, 0/9 functions; pack.hpp 55 dark regions;
//   the migrate/clear/vector accessors never run).
// - Expected bug classes: heap overflow and chunk-list corruption in
//   append_copy()/append_ref()/migrate() pointer and iovec-array arithmetic,
//   bytes corrupted on the wire (caught by the property below), and leaks in
//   the chunk lifecycle (clear/migrate/destructor).
// - Round-trip property: bytes produced by pack(obj) into a vrefbuffer,
//   reassembled from vector()/vector_size(), must be byte-identical to a
//   direct pack(obj) into an sbuffer (packer<Stream> funnels the same byte
//   sequence through both streams), and must unpack cleanly. Any mismatch
//   or parse failure means the vrefbuffer emitted corrupted bytes - a real
//   correctness/memory-safety finding, surfaced via abort().
// - The ref_size parameter (clamped by the library to >= packer_max_buffer_
//   size+1 = 10) decides copy-vs-reference near the 9-byte packer-temp
//   boundary and around real str/bin lengths; chunk_size forces
//   append_copy() chunk-chain growth; migrate() coalesces two buffers.
#include <msgpack.hpp>

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace {

// Minimal C++11 replacement for FuzzedDataProvider (whose bundled header no
// longer compiles under the project's -std=c++11 build convention): options
// are consumed from the tail of the input, the payload is what remains.
class byte_reader {
public:
    byte_reader(const std::uint8_t* data, std::size_t size)
        : d_(data), s_(size) {}

    std::uint64_t tail_u64() {
        std::uint64_t v = 0;
        for (int i = 0; i < 8 && s_ > 0; ++i) {
            --s_;
            v = (v << 8) | static_cast<std::uint64_t>(d_[s_]);
        }
        return v;
    }

    std::size_t tail_range(std::size_t lo, std::size_t hi) {
        return lo + static_cast<std::size_t>(
                        tail_u64() % static_cast<std::uint64_t>(hi - lo + 1));
    }

    std::uint8_t tail_u8() {
        if (s_ == 0) return 0;
        --s_;
        return d_[s_];
    }

    const std::uint8_t* data() const { return d_; }
    std::size_t remaining() const { return s_; }

private:
    const std::uint8_t* d_;
    std::size_t s_;
};

void sink_u64(std::size_t v) {
    static volatile std::size_t sink;
    sink = v;
}

}  // namespace

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 4) return 0;
    byte_reader br(data, size);
    const std::size_t ref_size = br.tail_range(10, 256);
    const std::size_t chunk_size = br.tail_range(16, 4104);
    const std::size_t ref_size2 = br.tail_range(10, 256);
    const std::size_t chunk_size2 = br.tail_range(16, 4104);
    const std::uint8_t ops = br.tail_u8();
    std::vector<std::uint8_t> payload(br.data(), br.data() + br.remaining());
    if (payload.empty()) return 0;

    try {
        msgpack::object_handle oh =
            msgpack::unpack(reinterpret_cast<const char*>(payload.data()),
                            payload.size(), MSGPACK_NULLPTR, MSGPACK_NULLPTR,
                            msgpack::unpack_limit(64, 1024, 1024, 4096, 4096,
                                                  4096));
        msgpack::object const obj = oh.get();

        msgpack::vrefbuffer buf(ref_size, chunk_size);
        msgpack::pack(buf, obj);

        msgpack::vrefbuffer migrated(ref_size2, chunk_size2);
        msgpack::vrefbuffer* tail = &buf;
        if (ops & 1) {
            buf.migrate(&migrated);
            tail = &migrated;
        }
        if (ops & 2) {
            // clear() then re-pack into the same buffer: chunk reuse path.
            buf.clear();
            msgpack::pack(buf, obj);
            tail = &buf;
        }

        // Reassemble the wire bytes from the iovec view.
        const msgpack::iovec* vec = tail->vector();
        const std::size_t n = tail->vector_size();
        std::size_t total = 0;
        for (std::size_t i = 0; i < n; ++i) total += vec[i].iov_len;
        std::string wire;
        wire.resize(total);
        std::size_t off = 0;
        for (std::size_t i = 0; i < n; ++i) {
            std::memcpy(&wire[off], vec[i].iov_base, vec[i].iov_len);
            off += vec[i].iov_len;
        }
        sink_u64(wire.size());

        // Reference bytes: direct pack of the same object.
        msgpack::sbuffer direct;
        msgpack::pack(direct, obj);

        if (wire.size() != direct.size() ||
            std::memcmp(wire.data(), direct.data(), direct.size()) != 0) {
            // vrefbuffer emitted bytes that differ from the canonical pack.
            abort();
        }

        // The reassembled wire must also be valid msgpack.
        msgpack::object_handle oh2 =
            msgpack::unpack(wire.data(), wire.size(), MSGPACK_NULLPTR,
                            MSGPACK_NULLPTR,
                            msgpack::unpack_limit(64, 1024, 1024, 4096, 4096,
                                                  4096));
        sink_u64(static_cast<std::size_t>(oh2.get().type));
        sink_u64(direct.size());
    } catch (...) {
        // parse_error on the fuzz payload, bad_alloc on absurd lengths:
        // documented behaviour. A parse failure of the reassembled wire is
        // impossible while the wire equals the direct pack, so abort() above
        // is the only failure path for the property itself.
    }
    return 0;
}
