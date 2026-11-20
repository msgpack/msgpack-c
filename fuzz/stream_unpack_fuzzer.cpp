// Fuzz harness: msgpack::v1::unpacker streaming deserialisation state machine.
//
// Threat model (why this harness should find bugs):
// - msgpack is a deserialiser: the bytes are attacker controlled in every
//   deployment (IPC, caches, RPC payloads). The production harness
//   (unpack_pack_fuzzer) only ever exercises the one-shot msgpack::unpack()
//   path, so the streaming unpacker state machine - the documented way to
//   consume bytes off a socket or file in chunks - is never fuzzed
//   (coverage report 20261108: include/msgpack/v1/unpack.hpp 12/1018 lines,
//   5/98 functions).
// - The attacker controls both the byte stream and, in real users, the chunk
//   boundaries it arrives in; this harness derives both from the fuzz input.
// - Expected bug classes: OOB reads/writes or use-after-free in
//   expand_buffer()/reserve_buffer() pointer arithmetic and in zone flush /
//   reference-count handling when str/bin/ext objects reference the internal
//   buffer (unpack_reference_func), wrong state transitions when a message
//   spans chunk boundaries (single-byte feeds), and hangs on adversarial
//   aggregate headers.
//
// Design notes:
// - Feed protocol follows the class contract exactly: reserve_buffer(want),
//   memcpy into buffer() clamped to buffer_capacity(), buffer_consumed(n).
// - unpack_limit fields are fuzz-chosen but bounded (depth<=64, aggregates
//   <=128, scalars<=4096) so limit enforcement is exercised without OOM.
// - initial_buffer_size is fuzz-chosen (16..65536) to stress expand_buffer().
// - Work is bounded: total fed <= 64 KiB, objects <= 4096.
// - msgpack::parse_error on invalid bytes is documented behaviour and is
//   caught; the swallow-all outer catch mirrors the incumbent
//   unpack_pack_fuzzer.
#include <msgpack.hpp>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>

namespace {

// Minimal C++11 replacement for FuzzedDataProvider (whose bundled header no
// longer compiles under the project's -std=c++11 build convention): options
// are consumed from the tail of the input, the payload is what remains.
class byte_reader {
public:
    byte_reader(const std::uint8_t* data, std::size_t size)
        : d_(data), s_(size) {}

    std::uint8_t tail_u8() {
        if (s_ == 0) return 0;
        --s_;
        return d_[s_];
    }

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

    int tail_range_int(int lo, int hi) {
        return lo + static_cast<int>(
                        tail_u64() % static_cast<std::uint64_t>(hi - lo + 1));
    }

    const std::uint8_t* data() const { return d_; }
    std::size_t remaining() const { return s_; }

private:
    const std::uint8_t* d_;
    std::size_t s_;
};

struct ref_decision {
    int mode;
};

// unpack_reference_func decides whether str/bin/ext values reference the
// unpacker's internal buffer (zero-copy) or are copied into the zone. The
// three modes exercise both answers and mixed per-type/per-length decisions.
bool fuzz_reference_func(msgpack::type::object_type type, std::size_t len,
                         void* user_data) {
    ref_decision* d = static_cast<ref_decision*>(user_data);
    switch (d->mode) {
        case 0: return true;
        case 1: return false;
        default: return (static_cast<unsigned>(type) + len) % 2 == 0;
    }
}

const std::size_t kMaxTotalFed = 64 * 1024;
const std::size_t kMaxObjects = 4096;

void sink_u64(std::size_t v) {
    static volatile std::size_t sink;
    sink = v;
}

}  // namespace

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 8) return 0;
    byte_reader br(data, size);
    const int ref_mode = br.tail_range_int(0, 2);
    const int initial_buf_exp = br.tail_range_int(4, 16);
    const int depth = br.tail_range_int(1, 64);
    const int aggregate = br.tail_range_int(1, 128);
    const int scalar = br.tail_range_int(1, 4096);
    const std::size_t chunk = br.tail_range(1, 512);
    const int tail_mode = br.tail_range_int(0, 3);
    const bool legacy_path = (br.tail_u8() & 1u) != 0;
    std::vector<uint8_t> payload(br.data(), br.data() + br.remaining());
    if (payload.empty()) return 0;
    const std::size_t total = std::min<std::size_t>(payload.size(), kMaxTotalFed);

    const msgpack::unpack_limit limit(
        depth, aggregate, aggregate, scalar, scalar, scalar);
    ref_decision ctx{ref_mode};

    if (!legacy_path) {
        // Documented streaming protocol.
        msgpack::v1::unpacker upk(fuzz_reference_func, &ctx,
                                  static_cast<std::size_t>(1) << initial_buf_exp,
                                  limit);
        std::size_t pos = 0;
        std::size_t objects = 0;
        try {
            while (pos < total && objects < kMaxObjects) {
                const std::size_t want = std::min(chunk, total - pos);
                upk.reserve_buffer(want);
                const std::size_t n = std::min(want, upk.buffer_capacity());
                if (n == 0) break;
                std::memcpy(upk.buffer(), payload.data() + pos, n);
                upk.buffer_consumed(n);
                pos += n;
                bool referenced = false;
                msgpack::object_handle oh;
                while (upk.next(oh, referenced)) {
                    ++objects;
                    sink_u64(static_cast<std::size_t>(oh.get().type));
                    if (objects >= kMaxObjects) break;
                }
                if (upk.nonparsed_size() > 0) {
                    switch (tail_mode) {
                        case 0: upk.remove_nonparsed_buffer(); break;
                        case 1: upk.skip_nonparsed_buffer(upk.nonparsed_size() / 2); break;
                        case 2: break;
                        default: upk.reset(); pos = total; break;
                    }
                }
            }
            sink_u64(upk.message_size() + upk.parsed_size() + upk.buffer_capacity());
        } catch (...) {
            // parse_error on invalid bytes: documented behaviour.
        }
    } else {
        // Legacy execute()/data()/release_zone()/reset_zone() interface
        // (backward-compat paths, dark in production coverage).
        try {
            msgpack::v1::unpacker upk(fuzz_reference_func, &ctx, 64, limit);
            std::size_t pos = 0;
            std::size_t objects = 0;
            while (pos < total && objects < kMaxObjects) {
                const std::size_t want = std::min<std::size_t>(256, total - pos);
                upk.reserve_buffer(want);
                const std::size_t n = std::min(want, upk.buffer_capacity());
                std::memcpy(upk.buffer(), payload.data() + pos, n);
                upk.buffer_consumed(n);
                pos += n;
                while (upk.execute() && objects < kMaxObjects) {
                    sink_u64(static_cast<std::size_t>(upk.data().type));
                    ++objects;
                    std::unique_ptr<msgpack::zone> z(upk.release_zone());
                    upk.reset_zone();
                }
            }
        } catch (...) {
        }
    }
    return 0;
}
