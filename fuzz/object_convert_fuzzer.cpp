// Fuzz harness: msgpack::object -> native type conversion surface.
//
// Threat model (why this harness should find bugs):
// - Applications that receive attacker-controlled msgpack immediately convert
//   msgpack::object values into native C++ types (as<T>() / convert()).
//   The production harness (unpack_pack_fuzzer) never converts: the whole
//   conversion surface is dark (report 20261108: v1/object.hpp 519 dark
//   lines, 109/140 functions dark; int/ext/fixint/wstring/float/raw/string
//   adaptors at 0%; vector/map/timespec adaptors are not even instantiated
//   in the coverage build).
// - Expected bug classes: type confusion in convert specialisations (wrong
//   union member read), integer overflow and float-cast overflow in numeric
//   range checks (UBSan), OOB in container conversions that walk object
//   arrays/maps, UTF-8 decoding bugs in the std::wstring adaptor, and
//   ext/raw length handling on crafted lengths.
// - Mismatched conversions throw msgpack::type_error: that is the documented
//   API contract, so each conversion is guarded individually and nothing is
//   silenced. Numeric sinks move raw bits (never value-cast) so the harness
//   itself cannot raise float-cast-overflow that the library is not
//   responsible for.
// - The zone deep-copy (msgpack::object(o, zone)) exercises the
//   object_with_zone clone path and instantiates the zone allocator, which
//   no harness reaches today.
#include <msgpack.hpp>

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <vector>

namespace {

// Minimal C++11 replacement for FuzzedDataProvider (whose bundled header no
// longer compiles under the project's -std=c++11 build convention): option
// bytes are consumed from the tail of the input, the payload is what remains.
class byte_reader {
public:
    byte_reader(const std::uint8_t* data, std::size_t size)
        : d_(data), s_(size) {}

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

void sink_double(double v) {
    std::uint64_t bits = 0;
    std::memcpy(&bits, &v, sizeof(bits));
    sink_u64(static_cast<std::size_t>(bits));
}

}  // namespace

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 4) return 0;
    byte_reader br(data, size);
    const std::uint8_t primary = br.tail_u8();
    const std::uint8_t secondary = br.tail_u8();
    const std::uint8_t container = br.tail_u8();
    const std::uint8_t option = br.tail_u8();
    std::vector<std::uint8_t> payload(br.data(), br.data() + br.remaining());
    if (payload.empty()) return 0;

    try {
        msgpack::object_handle oh =
            msgpack::unpack(reinterpret_cast<const char*>(payload.data()),
                            payload.size(), MSGPACK_NULLPTR, MSGPACK_NULLPTR,
                            msgpack::unpack_limit(64, 1024, 1024, 4096, 4096,
                                                  4096));
        msgpack::object const o = oh.get();
        sink_u64(static_cast<std::size_t>(o.type));
        sink_u64(o.is_nil() ? 1u : 0u);

        // Primary: as<T>() matrix over the dark adaptors. Mismatches throw
        // type_error (documented); each op is individually guarded so one
        // mismatched conversion does not shadow the others.
        try {
            switch (primary % 14) {
                case 0: { bool v = o.as<bool>(); sink_u64(v ? 1u : 0u); } break;
                case 1: { std::uint64_t v = o.as<std::uint64_t>(); sink_u64(static_cast<std::size_t>(v)); } break;
                case 2: { std::uint16_t v = o.as<std::uint16_t>(); sink_u64(v); } break;
                case 3: { std::uint8_t v = o.as<std::uint8_t>(); sink_u64(v); } break;
                case 4: { std::int64_t v = o.as<std::int64_t>(); sink_u64(static_cast<std::uint64_t>(v)); } break;
                case 5: { std::int8_t v = o.as<std::int8_t>(); sink_u64(static_cast<std::uint64_t>(static_cast<std::int64_t>(v))); } break;
                case 6: { float v = o.as<float>(); sink_double(v); } break;
                case 7: { double v = o.as<double>(); sink_double(v); } break;
                case 8: { std::string v = o.as<std::string>(); sink_u64(v.size()); } break;
                case 9: { std::wstring v = o.as<std::wstring>(); sink_u64(v.size()); } break;
                case 10: { msgpack::type::raw_ref v = o.as<msgpack::type::raw_ref>(); sink_u64(v.size); } break;
                case 11: { msgpack::type::ext v = o.as<msgpack::type::ext>(); sink_u64(v.size()); } break;
                case 12: { msgpack::type::ext_ref v = o.as<msgpack::type::ext_ref>(); sink_u64(v.size()); } break;
                case 13: {
                    // NOTE: this code base converts to global ::timespec
                    // (there is no msgpack::type::timespec here).
                    timespec v = o.as<timespec>();
                    sink_u64(static_cast<std::uint64_t>(v.tv_sec));
                    sink_u64(static_cast<std::uint64_t>(v.tv_nsec));
                } break;
            }
        } catch (msgpack::type_error const&) {}

        // Secondary: reference-form convert(), convert_if_not_nil(),
        // timespec reference conversion and v4raw_ref.
        try {
            switch (secondary % 6) {
                case 0: { std::string v; o.convert(v); sink_u64(v.size()); } break;
                case 1: { std::int32_t v = 0; o.convert(v); sink_u64(static_cast<std::uint64_t>(v)); } break;
                case 2: { std::string v; o.convert_if_not_nil(v); sink_u64(v.size()); } break;
                case 3: {
                    // timespec via the reference-form convert() (global
                    // ::timespec; msgpack::type::timespec does not exist here).
                    timespec v;
                    std::memset(&v, 0, sizeof(v));
                    o.convert(v);
                    sink_u64(static_cast<std::uint64_t>(v.tv_sec));
                } break;
                case 4: { msgpack::type::v4raw_ref v = o.as<msgpack::type::v4raw_ref>(); sink_u64(v.size); } break;
                case 5: {
                    // reference-form int64 conversion (implicit_type is
                    // protected in this code base, so use convert() directly)
                    std::int64_t v = 0;
                    o.convert(v);
                    sink_u64(static_cast<std::uint64_t>(v));
                } break;
            }
        } catch (msgpack::type_error const&) {}

        // Container conversions: instantiate adaptors absent from the
        // coverage build today (vector, map, recursion into element
        // adaptors). Deep-copy into a zone as well: object_with_zone clone
        // path plus the zone allocator.
        try {
            switch (container % 4) {
                case 0: { std::vector<int> v = o.as<std::vector<int>>(); sink_u64(v.size()); } break;
                case 1: { std::vector<std::string> v = o.as<std::vector<std::string>>(); sink_u64(v.size()); } break;
                case 2: { std::map<std::string, int> v = o.as<std::map<std::string, int>>(); sink_u64(v.size()); } break;
                case 3: { std::vector<char> v = o.as<std::vector<char>>(); sink_u64(v.size()); } break;
            }
        } catch (msgpack::type_error const&) {}

        try {
            msgpack::zone z;
            msgpack::object clone(o, z);
            sink_u64(static_cast<std::size_t>(clone.type));
            std::string s;
            clone.convert(s);
            sink_u64(s.size());
        } catch (msgpack::type_error const&) {}

        // Extra option bits: widen the numeric mix per input.
        if (option & 1) {
            try { std::uint32_t v = o.as<std::uint32_t>(); sink_u64(v); }
            catch (msgpack::type_error const&) {}
        }
        if (option & 2) {
            try { double v = o.as<double>(); sink_double(v); }
            catch (msgpack::type_error const&) {}
        }
        if (option & 4) {
            try { std::int16_t v = o.as<std::int16_t>(); sink_u64(static_cast<std::uint64_t>(v)); }
            catch (msgpack::type_error const&) {}
        }

    } catch (msgpack::parse_error const&) {
        // Invalid msgpack bytes: documented behaviour.
    } catch (...) {
        // bad_alloc and friends: documented behaviour.
    }
    return 0;
}
