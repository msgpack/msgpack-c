//
// MessagePack for C++ serializing routine
//
// Copyright (C) 2008-2013 FURUHASHI Sadayuki and KONDO Takatoshi
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
//
#ifndef MSGPACK_PACK_HPP
#define MSGPACK_PACK_HPP

#include "pack_define.h"
#include <stdexcept>
#include <limits.h>

namespace msgpack {


template <typename Stream>
class packer {
public:
	packer(Stream* s);
	packer(Stream& s);
	~packer();

public:
	template <typename T>
	packer<Stream>& pack(const T& v);

	packer<Stream>& pack_uint8(uint8_t d);
	packer<Stream>& pack_uint16(uint16_t d);
	packer<Stream>& pack_uint32(uint32_t d);
	packer<Stream>& pack_uint64(uint64_t d);
	packer<Stream>& pack_int8(int8_t d);
	packer<Stream>& pack_int16(int16_t d);
	packer<Stream>& pack_int32(int32_t d);
	packer<Stream>& pack_int64(int64_t d);

	packer<Stream>& pack_fix_uint8(uint8_t d);
	packer<Stream>& pack_fix_uint16(uint16_t d);
	packer<Stream>& pack_fix_uint32(uint32_t d);
	packer<Stream>& pack_fix_uint64(uint64_t d);
	packer<Stream>& pack_fix_int8(int8_t d);
	packer<Stream>& pack_fix_int16(int16_t d);
	packer<Stream>& pack_fix_int32(int32_t d);
	packer<Stream>& pack_fix_int64(int64_t d);

	packer<Stream>& pack_char(char d);
	packer<Stream>& pack_signed_char(signed char d);
	packer<Stream>& pack_short(short d);
	packer<Stream>& pack_int(int d);
	packer<Stream>& pack_long(long d);
	packer<Stream>& pack_long_long(long long d);
	packer<Stream>& pack_unsigned_char(unsigned char d);
	packer<Stream>& pack_unsigned_short(unsigned short d);
	packer<Stream>& pack_unsigned_int(unsigned int d);
	packer<Stream>& pack_unsigned_long(unsigned long d);
	packer<Stream>& pack_unsigned_long_long(unsigned long long d);

	packer<Stream>& pack_float(float d);
	packer<Stream>& pack_double(double d);

	packer<Stream>& pack_nil();
	packer<Stream>& pack_true();
	packer<Stream>& pack_false();

	packer<Stream>& pack_array(size_t n);

	packer<Stream>& pack_map(size_t n);

	packer<Stream>& pack_raw(size_t l);
	packer<Stream>& pack_raw_body(const char* b, size_t l);

private:
	template <typename T>
	void pack_real_uint8(T d);
	template <typename T>
	void pack_real_uint16(T d);
	template <typename T>
	void pack_real_uint32(T d);
	template <typename T>
	void pack_real_uint64(T d);
	template <typename T>
	void pack_real_int8(T d);
	template <typename T>
	void pack_real_int16(T d);
	template <typename T>
	void pack_real_int32(T d);
	template <typename T>
	void pack_real_int64(T d);

	void append_buffer(const unsigned char* buf, unsigned int len)
		{ m_stream.write((const char*)buf, len); }

private:
	Stream& m_stream;

private:
	packer();
};


template <typename Stream, typename T>
inline void pack(Stream* s, const T& v)
{
	packer<Stream>(s).pack(v);
}

template <typename Stream, typename T>
inline void pack(Stream& s, const T& v)
{
	packer<Stream>(s).pack(v);
}


#define msgpack_pack_inline_func(name) \
	template <typename Stream> \
	inline void packer<Stream>::_pack ## name

#define msgpack_pack_inline_func_cint(name) \
	template <typename Stream> \
	inline void packer<Stream>::_pack ## name

#define msgpack_pack_inline_func_fixint(name) \
	template <typename Stream> \
	inline void packer<Stream>::_pack_fix ## name

#define msgpack_pack_user Stream&

#define msgpack_pack_append_buffer append_buffer

#if defined(__LITTLE_ENDIAN__)
#define TAKE8_8(d)  ((uint8_t*)&d)[0]
#define TAKE8_16(d) ((uint8_t*)&d)[0]
#define TAKE8_32(d) ((uint8_t*)&d)[0]
#define TAKE8_64(d) ((uint8_t*)&d)[0]
#elif defined(__BIG_ENDIAN__)
#define TAKE8_8(d)  ((uint8_t*)&d)[0]
#define TAKE8_16(d) ((uint8_t*)&d)[1]
#define TAKE8_32(d) ((uint8_t*)&d)[3]
#define TAKE8_64(d) ((uint8_t*)&d)[7]
#endif

template <typename Stream>
packer<Stream>::packer(Stream* s) : m_stream(*s) { }

template <typename Stream>
packer<Stream>::packer(Stream& s) : m_stream(s) { }

template <typename Stream>
packer<Stream>::~packer() { }


template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_uint8(uint8_t d)
{ pack_real_uint8(d); return *this; }

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_uint16(uint16_t d)
{ pack_real_uint16(d); return *this; }

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_uint32(uint32_t d)
{ pack_real_uint32(d); return *this; }

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_uint64(uint64_t d)
{ pack_real_uint64(d); return *this; }

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_int8(int8_t d)
{ pack_real_int8(d); return *this; }

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_int16(int16_t d)
{ pack_real_int16(d); return *this; }

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_int32(int32_t d)
{ pack_real_int32(d); return *this; }

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_int64(int64_t d)
{ pack_real_int64(d); return *this;}


template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_fix_uint8(uint8_t d)
{
	unsigned char buf[2] = {0xcc, TAKE8_8(d)};
    append_buffer(buf, 2);
    return *this; 
}

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_fix_uint16(uint16_t d)
{ 
	unsigned char buf[3];
	buf[0] = 0xcd; _msgpack_store16(&buf[1], d);
    append_buffer(buf, 3);
    return *this;
}

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_fix_uint32(uint32_t d)
{ 
	unsigned char buf[5];
	buf[0] = 0xce; _msgpack_store32(&buf[1], d);
    append_buffer(buf, 5);
    return *this;
}

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_fix_uint64(uint64_t d)
{ 
	unsigned char buf[9];
	buf[0] = 0xcf; _msgpack_store64(&buf[1], d);
    append_buffer(buf, 9);
    return *this;
}

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_fix_int8(int8_t d)
{ 
	unsigned char buf[2] = {0xd0, TAKE8_8(d)};
    append_buffer(buf, 2);
    return *this;
}

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_fix_int16(int16_t d)
{ 
	unsigned char buf[3];
	buf[0] = 0xd1; _msgpack_store16(&buf[1], d);
    append_buffer(buf, 3);
    return *this;
}

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_fix_int32(int32_t d)
{ 
	unsigned char buf[5];
	buf[0] = 0xd2; _msgpack_store32(&buf[1], d);
    append_buffer(buf, 5);
    return *this;
}

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_fix_int64(int64_t d)
{ 
	unsigned char buf[9];
	buf[0] = 0xd3; _msgpack_store64(&buf[1], d);
    append_buffer(buf, 9);
    return *this;
}


template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_char(char d)
{ _pack_char(m_stream, d); return *this; }

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_signed_char(signed char d)
{ _pack_signed_char(m_stream, d); return *this; }

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_short(short d)
{
#if defined(SIZEOF_SHORT)
#if SIZEOF_SHORT == 2
	pack_real_int16(d);
#elif SIZEOF_SHORT == 4
	pack_real_int32(d);
#else
	pack_real_int64(d);
#endif

#elif defined(SHRT_MAX)
#if SHRT_MAX == 0x7fff
	pack_real_int16(d);
#elif SHRT_MAX == 0x7fffffff
	pack_real_int32(d);
#else
	pack_real_int64(d);
#endif

#else
if(sizeof(short) == 2) {
	pack_real_int16(d);
} else if(sizeof(short) == 4) {
	pack_real_int32(d);
} else {
	pack_real_int64(d);
}
#endif
	return *this;
}

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_int(int d)
{
#if defined(SIZEOF_INT)
#if SIZEOF_INT == 2
	pack_real_int16(d);
#elif SIZEOF_INT == 4
	pack_real_int32(d);
#else
	pack_real_int64(d);
#endif

#elif defined(INT_MAX)
#if INT_MAX == 0x7fff
	pack_real_int16(d);
#elif INT_MAX == 0x7fffffff
	pack_real_int32(d);
#else
	pack_real_int64(d);
#endif

#else
if(sizeof(int) == 2) {
	pack_real_int16(d);
} else if(sizeof(int) == 4) {
	pack_real_int32(d);
} else {
	pack_real_int64(d);
}
#endif
	return *this;
}

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_long(long d)
{
#if defined(SIZEOF_LONG)
#if SIZEOF_LONG == 2
	pack_real_int16(d);
#elif SIZEOF_LONG == 4
	pack_real_int32(d);
#else
	pack_real_int64(d);
#endif

#elif defined(LONG_MAX)
#if LONG_MAX == 0x7fffL
	pack_real_int16(d);
#elif LONG_MAX == 0x7fffffffL
	pack_real_int32(d);
#else
	pack_real_int64(d);
#endif

#else
if(sizeof(long) == 2) {
	pack_real_int16(d);
} else if(sizeof(long) == 4) {
	pack_real_int32(d);
} else {
	pack_real_int64(d);
}
#endif
	return *this;
}

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_long_long(long long d)
{
#if defined(SIZEOF_LONG_LONG)
#if SIZEOF_LONG_LONG == 2
	pack_real_int16(d);
#elif SIZEOF_LONG_LONG == 4
	pack_real_int32(d);
#else
	pack_real_int64(d);
#endif

#elif defined(LLONG_MAX)
#if LLONG_MAX == 0x7fffL
	pack_real_int16(d);
#elif LLONG_MAX == 0x7fffffffL
	pack_real_int32(d);
#else
	pack_real_int64(d);
#endif

#else
if(sizeof(long long) == 2) {
	pack_real_int16(d);
} else if(sizeof(long long) == 4) {
	pack_real_int32(d);
} else {
	pack_real_int64(d);
}
#endif
	return *this;
}


template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_unsigned_char(unsigned char d)
{ _pack_unsigned_char(m_stream, d); return *this; }

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_unsigned_short(unsigned short d)
{
#if defined(SIZEOF_SHORT)
#if SIZEOF_SHORT == 2
	pack_real_uint16(d);
#elif SIZEOF_SHORT == 4
	pack_real_uint32(d);
#else
	pack_real_uint64(d);
#endif

#elif defined(USHRT_MAX)
#if USHRT_MAX == 0xffffU
	pack_real_uint16(d);
#elif USHRT_MAX == 0xffffffffU
	pack_real_uint32(d);
#else
	pack_real_uint64(d);
#endif

#else
if(sizeof(unsigned short) == 2) {
	pack_real_uint16(d);
} else if(sizeof(unsigned short) == 4) {
	pack_real_uint32(d);
} else {
	pack_real_uint64(d);
}
#endif
	return *this;
}

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_unsigned_int(unsigned int d)
{
#if defined(SIZEOF_INT)
#if SIZEOF_INT == 2
	pack_real_uint16(d);
#elif SIZEOF_INT == 4
	pack_real_uint32(d);
#else
	pack_real_uint64(d);
#endif

#elif defined(UINT_MAX)
#if UINT_MAX == 0xffffU
	pack_real_uint16(d);
#elif UINT_MAX == 0xffffffffU
	pack_real_uint32(d);
#else
	pack_real_uint64(d);
#endif

#else
if(sizeof(unsigned int) == 2) {
	pack_real_uint16(d);
} else if(sizeof(unsigned int) == 4) {
	pack_real_uint32(d);
} else {
	pack_real_uint64(d);
}
#endif
	return *this;
}

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_unsigned_long(unsigned long d)
{
#if defined(SIZEOF_LONG)
#if SIZEOF_LONG == 2
	pack_real_uint16(d);
#elif SIZEOF_LONG == 4
	pack_real_uint32(d);
#else
	pack_real_uint64(d);
#endif

#elif defined(ULONG_MAX)
#if ULONG_MAX == 0xffffUL
	pack_real_uint16(d);
#elif ULONG_MAX == 0xffffffffUL
	pack_real_uint32(d);
#else
	pack_real_uint64(d);
#endif

#else
if(sizeof(unsigned long) == 2) {
	pack_real_uint16(d);
} else if(sizeof(unsigned long) == 4) {
	pack_real_uint32(d);
} else {
	pack_real_uint64(d);
}
#endif
	return *this;
}

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_unsigned_long_long(unsigned long long d)
{
#if defined(SIZEOF_LONG_LONG)
#if SIZEOF_LONG_LONG == 2
	pack_real_uint16(d);
#elif SIZEOF_LONG_LONG == 4
	pack_real_uint32(d);
#else
	pack_real_uint64(d);
#endif

#elif defined(ULLONG_MAX)
#if ULLONG_MAX == 0xffffUL
	pack_real_uint16(d);
#elif ULLONG_MAX == 0xffffffffUL
	pack_real_uint32(d);
#else
	pack_real_uint64(d);
#endif

#else
if(sizeof(unsigned long long) == 2) {
	pack_real_uint16(d);
} else if(sizeof(unsigned long long) == 4) {
	pack_real_uint32(d);
} else {
	pack_real_uint64(d);
}
#endif
	return *this;
}


template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_float(float d)
{
	union { float f; uint32_t i; } mem;
	mem.f = d;
	unsigned char buf[5];
	buf[0] = 0xca; _msgpack_store32(&buf[1], mem.i);
	append_buffer(buf, 5);
	return *this;
}

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_double(double d)
{
	union { double f; uint64_t i; } mem;
	mem.f = d;
	unsigned char buf[9];
	buf[0] = 0xcb;
#if defined(__arm__) && !(__ARM_EABI__) // arm-oabi
    // https://github.com/msgpack/msgpack-perl/pull/1
    mem.i = (mem.i & 0xFFFFFFFFUL) << 32UL | (mem.i >> 32UL);
#endif
    _msgpack_store64(&buf[1], mem.i);
	append_buffer(buf, 9);
	return *this;
}


template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_nil()
{
	static const unsigned char d = 0xc0;
	append_buffer(&d, 1);
	return *this;
}

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_true()
{
	static const unsigned char d = 0xc3;
	append_buffer(&d, 1);
	return *this;
}

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_false()
{
	static const unsigned char d = 0xc2;
	append_buffer(&d, 1);
	return *this;
}


template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_array(size_t n)
{
	if(n < 16) {
		unsigned char d = 0x90 | n;
		append_buffer(&d, 1);
	} else if(n < 65536) {
		unsigned char buf[3];
		buf[0] = 0xdc; _msgpack_store16(&buf[1], (uint16_t)n);
		append_buffer(buf, 3);
	} else {
		unsigned char buf[5];
		buf[0] = 0xdd; _msgpack_store32(&buf[1], (uint32_t)n);
		append_buffer(buf, 5);
	}
	return *this;
}

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_map(size_t n)
{
	if(n < 16) {
		unsigned char d = 0x80 | n;
		append_buffer(&TAKE8_8(d), 1);
	} else if(n < 65536) {
		unsigned char buf[3];
		buf[0] = 0xde; _msgpack_store16(&buf[1], (uint16_t)n);
		append_buffer(buf, 3);
	} else {
		unsigned char buf[5];
		buf[0] = 0xdf; _msgpack_store32(&buf[1], (uint32_t)n);
		append_buffer(buf, 5);
	}
	return *this;
}

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_raw(size_t l)
{
	if(l < 32) {
		unsigned char d = 0xa0 | (uint8_t)l;
		append_buffer(&TAKE8_8(d), 1);
	} else if(l < 65536) {
		unsigned char buf[3];
		buf[0] = 0xda; _msgpack_store16(&buf[1], (uint16_t)l);
		append_buffer(buf, 3);
	} else {
		unsigned char buf[5];
		buf[0] = 0xdb; _msgpack_store32(&buf[1], (uint32_t)l);
		append_buffer(buf, 5);
	}
	return *this;
}

template <typename Stream>
inline packer<Stream>& packer<Stream>::pack_raw_body(const char* b, size_t l)
{
	append_buffer((const unsigned char*)b, l);
	return *this;
}

template <typename Stream>
template <typename T>
inline void packer<Stream>::pack_real_uint8(T d) 
{ 
	if(d < (1<<7)) { 
		/* fixnum */ 
		append_buffer(&TAKE8_8(d), 1); 
	} else { 
		/* unsigned 8 */ 
		unsigned char buf[2] = {0xcc, TAKE8_8(d)}; 
		append_buffer(buf, 2); 
	} 
} 

template <typename Stream>
template <typename T>
inline void packer<Stream>::pack_real_uint16(T d) 
{ 
	if(d < (1<<7)) { 
		/* fixnum */ 
		append_buffer(&TAKE8_16(d), 1); 
	} else if(d < (1<<8)) { 
		/* unsigned 8 */ 
		unsigned char buf[2] = {0xcc, TAKE8_16(d)}; 
		append_buffer(buf, 2); 
	} else { 
		/* unsigned 16 */ 
		unsigned char buf[3]; 
		buf[0] = 0xcd; _msgpack_store16(&buf[1], (uint16_t)d); 
		append_buffer(buf, 3); 
	} 
} 

template <typename Stream>
template <typename T>
inline void packer<Stream>::pack_real_uint32(T d) 
{ 
	if(d < (1<<8)) { 
		if(d < (1<<7)) { 
			/* fixnum */ 
			append_buffer(&TAKE8_32(d), 1); 
		} else { 
			/* unsigned 8 */ 
			unsigned char buf[2] = {0xcc, TAKE8_32(d)}; 
			append_buffer(buf, 2); 
		} 
	} else { 
		if(d < (1<<16)) { 
			/* unsigned 16 */ 
			unsigned char buf[3]; 
			buf[0] = 0xcd; _msgpack_store16(&buf[1], (uint16_t)d); 
			append_buffer(buf, 3); 
		} else { 
			/* unsigned 32 */ 
			unsigned char buf[5]; 
			buf[0] = 0xce; _msgpack_store32(&buf[1], (uint32_t)d); 
			append_buffer(buf, 5); 
		} 
	} 
} 

template <typename Stream>
template <typename T>
inline void packer<Stream>::pack_real_uint64(T d) 
{ 
	if(d < (1ULL<<8)) { 
		if(d < (1ULL<<7)) { 
			/* fixnum */ 
			append_buffer(&TAKE8_64(d), 1); 
		} else { 
			/* unsigned 8 */ 
			unsigned char buf[2] = {0xcc, TAKE8_64(d)}; 
			append_buffer(buf, 2); 
		} 
	} else { 
		if(d < (1ULL<<16)) { 
			/* unsigned 16 */ 
			unsigned char buf[3]; 
			buf[0] = 0xcd; _msgpack_store16(&buf[1], (uint16_t)d); 
			append_buffer(buf, 3); 
		} else if(d < (1ULL<<32)) { 
			/* unsigned 32 */ 
			unsigned char buf[5]; 
			buf[0] = 0xce; _msgpack_store32(&buf[1], (uint32_t)d); 
			append_buffer(buf, 5); 
		} else { 
			/* unsigned 64 */ 
			unsigned char buf[9]; 
			buf[0] = 0xcf; _msgpack_store64(&buf[1], d); 
			append_buffer(buf, 9); 
		} 
	} 
} 

template <typename Stream>
template <typename T>
inline void packer<Stream>::pack_real_int8(T d) 
{ 
	if(d < -(1<<5)) { 
		/* signed 8 */ 
		unsigned char buf[2] = {0xd0, TAKE8_8(d)}; 
		append_buffer(buf, 2); 
	} else { 
		/* fixnum */ 
		append_buffer(&TAKE8_8(d), 1); 
	} 
} 

template <typename Stream>
template <typename T>
inline void packer<Stream>::pack_real_int16(T d) 
{ 
	if(d < -(1<<5)) { 
		if(d < -(1<<7)) { 
			/* signed 16 */ 
			unsigned char buf[3]; 
			buf[0] = 0xd1; _msgpack_store16(&buf[1], (int16_t)d); 
			append_buffer(buf, 3); 
		} else { 
			/* signed 8 */ 
			unsigned char buf[2] = {0xd0, TAKE8_16(d)}; 
			append_buffer(buf, 2); 
		} 
	} else if(d < (1<<7)) { 
		/* fixnum */ 
		append_buffer(&TAKE8_16(d), 1); 
	} else { 
		if(d < (1<<8)) { 
			/* unsigned 8 */ 
			unsigned char buf[2] = {0xcc, TAKE8_16(d)}; 
			append_buffer(buf, 2); 
		} else { 
			/* unsigned 16 */ 
			unsigned char buf[3]; 
			buf[0] = 0xcd; _msgpack_store16(&buf[1], (uint16_t)d); 
			append_buffer(buf, 3); 
		} 
	} 
} 

template <typename Stream>
template <typename T>
inline void packer<Stream>::pack_real_int32(T d) 
{ 
	if(d < -(1<<5)) { 
		if(d < -(1<<15)) { 
			/* signed 32 */ 
			unsigned char buf[5]; 
			buf[0] = 0xd2; _msgpack_store32(&buf[1], (int32_t)d); 
			append_buffer(buf, 5); 
		} else if(d < -(1<<7)) { 
			/* signed 16 */ 
			unsigned char buf[3]; 
			buf[0] = 0xd1; _msgpack_store16(&buf[1], (int16_t)d); 
			append_buffer(buf, 3); 
		} else { 
			/* signed 8 */ 
			unsigned char buf[2] = {0xd0, TAKE8_32(d)}; 
			append_buffer(buf, 2); 
		} 
	} else if(d < (1<<7)) { 
		/* fixnum */ 
		append_buffer(&TAKE8_32(d), 1); 
	} else { 
		if(d < (1<<8)) { 
			/* unsigned 8 */ 
			unsigned char buf[2] = {0xcc, TAKE8_32(d)}; 
			append_buffer(buf, 2); 
		} else if(d < (1<<16)) { 
			/* unsigned 16 */ 
			unsigned char buf[3]; 
			buf[0] = 0xcd; _msgpack_store16(&buf[1], (uint16_t)d); 
			append_buffer(buf, 3); 
		} else { 
			/* unsigned 32 */ 
			unsigned char buf[5]; 
			buf[0] = 0xce; _msgpack_store32(&buf[1], (uint32_t)d); 
			append_buffer(buf, 5); 
		} 
	} 
} 

template <typename Stream>
template <typename T>
inline void packer<Stream>::pack_real_int64(T d) 
{ 
	if(d < -(1LL<<5)) { 
		if(d < -(1LL<<15)) { 
			if(d < -(1LL<<31)) { 
				/* signed 64 */ 
				unsigned char buf[9]; 
				buf[0] = 0xd3; _msgpack_store64(&buf[1], d); 
				append_buffer(buf, 9); 
			} else { 
				/* signed 32 */ 
				unsigned char buf[5]; 
				buf[0] = 0xd2; _msgpack_store32(&buf[1], (int32_t)d); 
				append_buffer(buf, 5); 
			} 
		} else { 
			if(d < -(1<<7)) { 
				/* signed 16 */ 
				unsigned char buf[3]; 
				buf[0] = 0xd1; _msgpack_store16(&buf[1], (int16_t)d); 
				append_buffer(buf, 3); 
			} else { 
				/* signed 8 */ 
				unsigned char buf[2] = {0xd0, TAKE8_64(d)}; 
				append_buffer(buf, 2); 
			} 
		} 
	} else if(d < (1<<7)) { 
		/* fixnum */ 
		append_buffer(&TAKE8_64(d), 1); 
	} else { 
		if(d < (1LL<<16)) { 
			if(d < (1<<8)) { 
				/* unsigned 8 */ 
				unsigned char buf[2] = {0xcc, TAKE8_64(d)}; 
				append_buffer(buf, 2); 
			} else { 
				/* unsigned 16 */ 
				unsigned char buf[3]; 
				buf[0] = 0xcd; _msgpack_store16(&buf[1], (uint16_t)d); 
				append_buffer(buf, 3); 
			} 
		} else { 
			if(d < (1LL<<32)) { 
				/* unsigned 32 */ 
				unsigned char buf[5]; 
				buf[0] = 0xce; _msgpack_store32(&buf[1], (uint32_t)d); 
				append_buffer(buf, 5); 
			} else { 
				/* unsigned 64 */ 
				unsigned char buf[9]; 
				buf[0] = 0xcf; _msgpack_store64(&buf[1], d); 
				append_buffer(buf, 9); 
			} 
		} 
	} 
} 

}  // namespace msgpack

#endif /* msgpack/pack.hpp */

