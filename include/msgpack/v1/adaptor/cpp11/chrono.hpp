//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2017 KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MSGPACK_V1_TYPE_CPP11_CHRONO_HPP
#define MSGPACK_V1_TYPE_CPP11_CHRONO_HPP

#include "msgpack/versioning.hpp"
#include "msgpack/adaptor/adaptor_base.hpp"
#include "msgpack/object.hpp"
#include "msgpack/adaptor/check_container_size.hpp"

#include <limits>
#include <chrono>

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond

namespace adaptor {

namespace detail {
template <
    typename Target,
    typename Source,
    bool target_is_signed = std::is_signed<Target>::value,
    bool source_is_signed = std::is_signed<Source>::value,
    typename = typename std::enable_if<
                   std::is_integral<Target>::value &&
                   std::is_integral<Source>::value
               >::type
>
struct would_underflow {
    // The default case includes the cases that Source being unsigned, and since Source
    // is unsigned, no underflow can happen
    would_underflow(Source) : value{false} {}
    bool value;
};

template <typename Target, typename Source>
struct would_underflow<Target, Source, false, true> {
    // When Source is signed and Target is unsigned, we only need to compare with 0 to
    // detect underflow, this works correctly and also avoids warnings from the compiler
    would_underflow(Source source) : value{source < 0} {}
    bool value;
};
template <typename Target, typename Source>
struct would_underflow<Target, Source, true, true> {
    // When Source and Target are signed, the promotion rules apply sensibly so we do
    // not need to do anything
    would_underflow(Source source)
        : value{source < std::numeric_limits<Target>::min()} {}
    bool value;
};

template <
    typename Target,
    typename Source,
    bool target_is_signed = std::is_signed<Target>::value,
    bool source_is_signed = std::is_signed<Source>::value,
    typename = typename std::enable_if<
                   std::is_integral<Target>::value &&
                   std::is_integral<Source>::value
               >::type
>
struct would_overflow {
    // The default case is Source and Target having the same signedness, the promotion
    // rule also apply sensibly here so nothing special needs to be done
    would_overflow(Source source)
        : value{source > std::numeric_limits<Target>::max()} {}
    bool value;
};
template <typename Target, typename Source>
struct would_overflow <Target, Source, false, true> {
    // When Target is unsigned and Source is signed, we cannot rely on the promotion
    // rule.
    would_overflow(Source source)
        : value{
              sizeof(Target) >= sizeof(Source)
              // Given Source is signed, Target being unsigned and having at least the
              // same size makes impossible to overflow
              ? false
              // Source being larger than Target makes it safe to cast the maximum value
              // of Target to Source
              : source > static_cast<Source>(std::numeric_limits<Target>::max())
          } {}
    bool value;
};
template <typename Target, typename Source>
struct would_overflow <Target, Source, true, false> {
    // When Target is signed and Source is unsigned, we cannot rely on the promotion
    // rule.
    would_overflow(Source source)
        : value{
              sizeof(Target) > sizeof(Source)
              // Target being larger than Source makes it impossible to overflow
              ? false
              // Source being unsigned and having at least the size of Target makes it
              // safe to cast the maximum value of Target to Source
              : source > static_cast<Source>(std::numeric_limits<Target>::max())
          } {}
    bool value;
};

template <
    typename Target,
    typename Source,
    typename = typename std::enable_if<
                   std::is_integral<Target>::value &&
                   std::is_integral<Source>::value
               >::type
>
Target integral_cast(Source source) {
    if (would_underflow<Target, Source>(source).value) {
        throw std::underflow_error{
            "casting from Source to Target causes an underflow error"
        };
    }
    if(would_overflow<Target, Source>(source).value) {
        throw std::overflow_error{
            "casting from Source to Target causes an overflow error"
        };
    }

    return static_cast<Target>(source);
}

// Adds d to tp without overflowing Duration's integer rep. The seconds field of
// a timestamp comes straight off the wire, so scaling it to Duration's ticks
// (1e9 for nanoseconds, 1e12 for picoseconds) can exceed the rep's range.
template <typename Clock, typename Duration, typename Rep, typename Period>
void checked_add(
    std::chrono::time_point<Clock, Duration>& tp,
    std::chrono::duration<Rep, Period> d,
    std::false_type /* floating point rep */) {
    typedef typename Duration::rep rep;
    typedef std::ratio_divide<Period, typename Duration::period> ratio;
    static_assert(ratio::den == 1, "d must convert to Duration exactly");
    rep const max = std::chrono::duration_values<rep>::max();
    rep const min = std::chrono::duration_values<rep>::min();
    rep const factor = static_cast<rep>(ratio::num);
    rep v = integral_cast<rep>(d.count());
    if (v > max / factor) {
        throw std::overflow_error{"timestamp does not fit the time_point"};
    }
    if (v < min / factor) {
        throw std::underflow_error{"timestamp does not fit the time_point"};
    }
    v *= factor;
    rep const cur = tp.time_since_epoch().count();
    if (v > 0 && cur > max - v) {
        throw std::overflow_error{"timestamp does not fit the time_point"};
    }
    if (v < 0 && cur < min - v) {
        throw std::underflow_error{"timestamp does not fit the time_point"};
    }
    tp += Duration(v);
}

template <typename Clock, typename Duration, typename Rep, typename Period>
void checked_add(
    std::chrono::time_point<Clock, Duration>& tp,
    std::chrono::duration<Rep, Period> d,
    std::true_type /* floating point rep */) {
    tp += d;
}

template <typename Clock, typename Duration, typename Rep, typename Period>
void checked_add(
    std::chrono::time_point<Clock, Duration>& tp,
    std::chrono::duration<Rep, Period> d) {
    checked_add(tp, d, std::integral_constant<bool,
        std::chrono::treat_as_floating_point<typename Duration::rep>::value>());
}
} // namespace detail

template <typename Clock, typename Duration>
struct as<std::chrono::time_point<Clock, Duration>> {
    typename std::chrono::time_point<Clock, Duration> operator()(msgpack::object const& o) const {
        if(o.type != msgpack::type::EXT) { throw msgpack::type_error(); }
        if(o.via.ext.type() != -1) { throw msgpack::type_error(); }
        std::chrono::time_point<Clock, Duration> tp;
        switch(o.via.ext.size) {
        case 4: {
            uint32_t sec;
            _msgpack_load32(uint32_t, o.via.ext.data(), &sec);
            detail::checked_add(tp, std::chrono::seconds(sec));
        } break;
        case 8: {
            uint64_t value;
            _msgpack_load64(uint64_t, o.via.ext.data(), &value);
            uint32_t nanosec = detail::integral_cast<uint32_t>(value >> 34);
            uint64_t sec = value & 0x00000003ffffffffLL;
            detail::checked_add(tp, std::chrono::duration_cast<Duration>(
                std::chrono::nanoseconds(nanosec)));
            detail::checked_add(tp, std::chrono::seconds(sec));
        } break;
        case 12: {
            uint32_t nanosec;
            _msgpack_load32(uint32_t, o.via.ext.data(), &nanosec);
            int64_t sec;
            _msgpack_load64(int64_t, o.via.ext.data() + 4, &sec);

            if (sec >= 0) {
                detail::checked_add(tp, std::chrono::seconds(sec));
                detail::checked_add(tp, std::chrono::duration_cast<Duration>(
                    std::chrono::nanoseconds(nanosec)));
            }
            else {
                if (nanosec == 0) {
                    detail::checked_add(tp, std::chrono::seconds(sec));
                }
                else {
                    ++sec;
                    detail::checked_add(tp, std::chrono::seconds(sec));
                    int64_t ns = detail::integral_cast<int64_t>(nanosec) - 1000000000L;
                    detail::checked_add(tp, std::chrono::duration_cast<Duration>(
                        std::chrono::nanoseconds(ns)));
                }
            }
        } break;
        default:
            throw msgpack::type_error();
        }
        return tp;
    }
};

template <typename Clock, typename Duration>
struct convert<std::chrono::time_point<Clock, Duration>> {
    msgpack::object const& operator()(msgpack::object const& o, std::chrono::time_point<Clock, Duration>& v) const {
        if(o.type != msgpack::type::EXT) { throw msgpack::type_error(); }
        if(o.via.ext.type() != -1) { throw msgpack::type_error(); }
        std::chrono::time_point<Clock, Duration> tp;
        switch(o.via.ext.size) {
        case 4: {
            uint32_t sec;
            _msgpack_load32(uint32_t, o.via.ext.data(), &sec);
            detail::checked_add(tp, std::chrono::seconds(sec));
            v = tp;
        } break;
        case 8: {
            uint64_t value;
            _msgpack_load64(uint64_t, o.via.ext.data(), &value);
            uint32_t nanosec = detail::integral_cast<uint32_t>(value >> 34);
            uint64_t sec = value & 0x00000003ffffffffLL;
            detail::checked_add(tp, std::chrono::duration_cast<Duration>(
                std::chrono::nanoseconds(nanosec)));
            detail::checked_add(tp, std::chrono::seconds(sec));
            v = tp;
        } break;
        case 12: {
            uint32_t nanosec;
            _msgpack_load32(uint32_t, o.via.ext.data(), &nanosec);
            int64_t sec;
            _msgpack_load64(int64_t, o.via.ext.data() + 4, &sec);

            if (sec >= 0) {
                detail::checked_add(tp, std::chrono::seconds(sec));
                detail::checked_add(tp, std::chrono::duration_cast<Duration>(
                    std::chrono::nanoseconds(nanosec)));
            }
            else {
                if (nanosec == 0) {
                    detail::checked_add(tp, std::chrono::seconds(sec));
                }
                else {
                    ++sec;
                    detail::checked_add(tp, std::chrono::seconds(sec));
                    int64_t ns = detail::integral_cast<int64_t>(nanosec) - 1000000000L;
                    detail::checked_add(tp, std::chrono::duration_cast<Duration>(
                        std::chrono::nanoseconds(ns)));
                }
            }

            v = tp;
        } break;
        default:
            throw msgpack::type_error();
        }
        return o;
    }
};

template <typename Clock, typename Duration>
struct pack<std::chrono::time_point<Clock, Duration>> {
    template <typename Stream>
    msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, std::chrono::time_point<Clock, Duration> const& v) const {
        std::chrono::seconds whole_sec =
            std::chrono::duration_cast<std::chrono::seconds>(v.time_since_epoch());
        int64_t nanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(
            v.time_since_epoch() - whole_sec).count();
        int64_t sec = whole_sec.count();
        if (nanosec < 0) {
            nanosec = 1000000000L + nanosec;
            --sec;
        }

        if ((sec >> 34) == 0) {
            uint64_t data64 = (detail::integral_cast<uint64_t>(nanosec) << 34) | detail::integral_cast<uint64_t>(sec);
            if ((data64 & 0xffffffff00000000L) == 0) {
                // timestamp 32
                o.pack_ext(4, -1);
                uint32_t data32 = detail::integral_cast<uint32_t>(data64);
                char buf[4];
                _msgpack_store32(buf, data32);
                o.pack_ext_body(buf, 4);
            }
            else {
                // timestamp 64
                o.pack_ext(8, -1);
                char buf[8];
                _msgpack_store64(buf, data64);
                o.pack_ext_body(buf, 8);
            }
        }
        else {
            // timestamp 96
            o.pack_ext(12, -1);
            char buf[12];


            _msgpack_store32(&buf[0], detail::integral_cast<uint32_t>(nanosec));
            _msgpack_store64(&buf[4], sec);
            o.pack_ext_body(buf, 12);
        }
        return o;
    }
};

template <typename Clock, typename Duration>
struct object_with_zone<std::chrono::time_point<Clock, Duration>> {
    void operator()(msgpack::object::with_zone& o, const std::chrono::time_point<Clock, Duration>& v) const {
        std::chrono::seconds whole_sec =
            std::chrono::duration_cast<std::chrono::seconds>(v.time_since_epoch());
        int64_t nanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(
            v.time_since_epoch() - whole_sec).count();
        int64_t sec = whole_sec.count();
        if (nanosec < 0) {
            nanosec = 1000000000L + nanosec;
            --sec;
        }
        if ((sec >> 34) == 0) {
            uint64_t data64 = (detail::integral_cast<uint64_t>(nanosec) << 34) | detail::integral_cast<uint64_t>(sec);
            if ((data64 & 0xffffffff00000000L) == 0) {
                // timestamp 32
                o.type = msgpack::type::EXT;
                o.via.ext.size = 4;
                char* p = static_cast<char*>(o.zone.allocate_no_align(o.via.ext.size + 1));
                p[0] = static_cast<char>(-1);
                uint32_t data32 = detail::integral_cast<uint32_t>(data64);
                _msgpack_store32(&p[1], data32);
                o.via.ext.ptr = p;
            }
            else {
                // timestamp 64
                o.type = msgpack::type::EXT;
                o.via.ext.size = 8;
                char* p = static_cast<char*>(o.zone.allocate_no_align(o.via.ext.size + 1));
                p[0] = static_cast<char>(-1);
                _msgpack_store64(&p[1], data64);
                o.via.ext.ptr = p;
            }
        }
        else {
            // timestamp 96
            o.type = msgpack::type::EXT;
            o.via.ext.size = 12;
            char* p = static_cast<char*>(o.zone.allocate_no_align(o.via.ext.size + 1));
            p[0] = static_cast<char>(-1);
            _msgpack_store32(&p[1], detail::integral_cast<uint32_t>(nanosec));
            _msgpack_store64(&p[1 + 4], sec);
            o.via.ext.ptr = p;
        }
    }
};

} // namespace adaptor

/// @cond
} // MSGPACK_API_VERSION_NAMESPACE(v1)
/// @endcond

} // namespace msgpack

#endif // MSGPACK_V1_TYPE_CPP11_CHRONO_HPP
