//
// MessagePack for C++ deserializing routine
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
#ifndef MSGPACK_UNPACK_HPP
#define MSGPACK_UNPACK_HPP

#include "object.hpp"
#include "zone.hpp"
#include "unpack_define.h"
#include "cpp_config.hpp"

#include <memory>
#include <stdexcept>
#include <byteswap.h>



#define COUNTER_SIZE (sizeof(_msgpack_atomic_counter_t))

#ifndef MSGPACK_UNPACKER_INIT_BUFFER_SIZE
#define MSGPACK_UNPACKER_INIT_BUFFER_SIZE (64*1024)
#endif

#ifndef MSGPACK_UNPACKER_RESERVE_SIZE
#define MSGPACK_UNPACKER_RESERVE_SIZE (32*1024)
#endif


// backward compatibility
#ifndef MSGPACK_UNPACKER_DEFAULT_INITIAL_BUFFER_SIZE
#define MSGPACK_UNPACKER_DEFAULT_INITIAL_BUFFER_SIZE MSGPACK_UNPACKER_INIT_BUFFER_SIZE
#endif


namespace msgpack {

namespace detail {

class unpack_user {
public:
	msgpack::zone const& zone() const { return *m_zone; }
	msgpack::zone& zone() { return *m_zone; }
	void set_zone(msgpack::zone& zone) { m_zone = &zone; }
	bool referenced() const { return m_referenced; }
	void set_referenced(bool referenced) { m_referenced = referenced; }
private:
	msgpack::zone* m_zone;
	bool m_referenced;
};

inline void unpack_uint8(uint8_t d, object& o)
{ o.type = type::POSITIVE_INTEGER; o.via.u64 = d; }

inline void unpack_uint16(uint16_t d, object& o)
{ o.type = type::POSITIVE_INTEGER; o.via.u64 = d; }

inline void unpack_uint32(uint32_t d, object& o)
{ o.type = type::POSITIVE_INTEGER; o.via.u64 = d; }

inline void unpack_uint64(uint64_t d, object& o)
{ o.type = type::POSITIVE_INTEGER; o.via.u64 = d; }

inline void unpack_int8(int8_t d, object& o)
{ if(d >= 0) { o.type = type::POSITIVE_INTEGER; o.via.u64 = d; }
		else { o.type = type::NEGATIVE_INTEGER; o.via.i64 = d; } }

inline void unpack_int16(int16_t d, object& o)
{ if(d >= 0) { o.type = type::POSITIVE_INTEGER; o.via.u64 = d; }
		else { o.type = type::NEGATIVE_INTEGER; o.via.i64 = d; } }

inline void unpack_int32(int32_t d, object& o)
{ if(d >= 0) { o.type = type::POSITIVE_INTEGER; o.via.u64 = d; }
		else { o.type = type::NEGATIVE_INTEGER; o.via.i64 = d; } }

inline void unpack_int64(int64_t d, object& o)
{ if(d >= 0) { o.type = type::POSITIVE_INTEGER; o.via.u64 = d; }
		else { o.type = type::NEGATIVE_INTEGER; o.via.i64 = d; } }

inline void unpack_float(float d, object& o)
{ o.type = type::DOUBLE; o.via.dec = d; }

inline void unpack_double(double d, object& o)
{ o.type = type::DOUBLE; o.via.dec = d; }

inline void unpack_nil(object& o)
{ o.type = type::NIL; }

inline void unpack_true(object& o)
{ o.type = type::BOOLEAN; o.via.boolean = true; }

inline void unpack_false(object& o)
{ o.type = type::BOOLEAN; o.via.boolean = false; }

struct unpack_array {
	bool operator()(unpack_user&u, unsigned int n, object& o) const {
		o.type = type::ARRAY;
		o.via.array.size = 0;
		o.via.array.ptr = static_cast<object*>(u.zone().allocate_align(n*sizeof(object)));
		if(o.via.array.ptr == nullptr) { return false; }
		return true;
	}
};

inline void unpack_array_item(object& c, object const& o)
{
#if defined(__GNUC__) && !defined(__clang__)
	memcpy(&c.via.array.ptr[c.via.array.size++], &o, sizeof(object));
#else  /* __GNUC__ && !__clang__ */
	c.via.array.ptr[c.via.array.size++] = o;
#endif /* __GNUC__ && !__clang__ */
}

struct unpack_map {
	bool operator()(unpack_user& u, unsigned int n, object& o) const {
		o.type = type::MAP;
		o.via.map.size = 0;
		o.via.map.ptr = static_cast<object_kv*>(u.zone().allocate_align(n*sizeof(object_kv)));
		if(o.via.map.ptr == nullptr) { return false; }
		return true;
	}
};

inline void unpack_map_item(object& c, object const& k, object const& v)
{
#if defined(__GNUC__) && !defined(__clang__)
	memcpy(&c.via.map.ptr[c.via.map.size].key, &k, sizeof(object));
	memcpy(&c.via.map.ptr[c.via.map.size].val, &v, sizeof(object));
#else  /* __GNUC__ && !__clang__ */
	c.via.map.ptr[c.via.map.size].key = k;
	c.via.map.ptr[c.via.map.size].val = v;
#endif /* __GNUC__ && !__clang__ */
	++c.via.map.size;
}

inline void unpack_str(unpack_user& u, const char* b, const char* p, unsigned int l, object& o)
{
	o.type = type::STR;
	o.via.str.ptr = p;
	o.via.str.size = l;
	u.set_referenced(true);
}

inline void unpack_bin(unpack_user& u, const char* b, const char* p, unsigned int l, object& o)
{
	o.type = type::BIN;
	o.via.bin.ptr = p;
	o.via.bin.size = l;
	u.set_referenced(true);
}


class unpack_stack {
public:
	object const& obj() const { return m_obj; }
	object& obj() { return m_obj; }
	void set_obj(object const& obj) { m_obj = obj; }
	size_t count() const { return m_count; }
	void set_count(size_t count) { m_count = count; }
	size_t decl_count() { return --m_count; }
	unsigned int container_type() const { return m_container_type; }
	void set_container_type(unsigned int container_type) { m_container_type = container_type; }
	object const& map_key() const { return m_map_key; }
	void set_map_key(object const& map_key) { m_map_key = map_key; }
private:
	object m_obj;
	size_t m_count;
	unsigned int m_container_type;
	object m_map_key;
};

inline void init_count(void* buffer)
{
	*reinterpret_cast<volatile _msgpack_atomic_counter_t*>(buffer) = 1;
}

inline void decl_count(void* buffer)
{
	if(_msgpack_sync_decr_and_fetch(reinterpret_cast<volatile _msgpack_atomic_counter_t*>(buffer)) == 0) {
		free(buffer);
	}
}

inline void incr_count(void* buffer)
{
	_msgpack_sync_incr_and_fetch(reinterpret_cast<volatile _msgpack_atomic_counter_t*>(buffer));
}

inline _msgpack_atomic_counter_t get_count(void* buffer)
{
	return *reinterpret_cast<volatile _msgpack_atomic_counter_t*>(buffer);
}

struct fix_tag {
	char f1[65]; // FIXME unique size is required. or use is_same meta function.
};

template <typename T>
struct value {
	typedef T type;
};
template <>
struct value<fix_tag> {
	typedef unsigned int type;
};

template <typename T>
inline void load(unsigned int& dst, const char* n, typename msgpack::enable_if<sizeof(T) == sizeof(fix_tag)>::type* = nullptr) {
	dst = static_cast<unsigned int>(*reinterpret_cast<const uint8_t*>(n)) & 0x0f;
}

template <typename T>
inline void load(T& dst, const char* n, typename msgpack::enable_if<sizeof(T) == 1>::type* = nullptr) {
	dst = static_cast<T>(*reinterpret_cast<const uint8_t*>(n));
}

template <typename T>
inline void load(T& dst, const char* n, typename msgpack::enable_if<sizeof(T) == 2>::type* = nullptr) {
	_msgpack_load16(T, n, &dst);
}

template <typename T>
inline void load(T& dst, const char* n, typename msgpack::enable_if<sizeof(T) == 4>::type* = nullptr) {
	_msgpack_load32(T, n, &dst);
}

template <typename T>
inline void load(T& dst, const char* n, typename msgpack::enable_if<sizeof(T) == 8>::type* = nullptr) {
	_msgpack_load64(T, n, &dst);
}

class context {
public:
	context():m_trail(0), m_cs(CS_HEADER), m_top(0)
	{
		m_stack[0].set_obj(object());
	}

	void init()
	{
		m_cs = CS_HEADER;
		m_trail = 0;
		m_top = 0;
		m_stack[0].set_obj(object());
	}

	object const& data() const
	{
		return m_stack[0].obj();
	}

	unpack_user& user()
	{
		return m_user;
	}

	unpack_user const& user() const
	{
		return m_user;
	}

	int execute(const char* data, size_t len, size_t& off)
	{
		assert(len >= off);

		m_start = data;
		m_current = data + off;
		m_stack_idx = 0;
		const char* const pe = data + len;
		const char* n = nullptr;

		object obj;

		if(m_current == pe) {
			off = m_current - m_start;
			return 0;
		}
		bool fixed_trail_again = false;
		do {
			if (m_cs == CS_HEADER) {
				fixed_trail_again = false;
				int selector = *reinterpret_cast<const unsigned char*>(m_current);
				if (0) {
				} else if(0x00 <= selector && selector <= 0x7f) { // Positive Fixnum
					unpack_uint8(*reinterpret_cast<const uint8_t*>(m_current), obj);
					int ret = push_proc(obj, off);
					if (ret != 0) return ret;
				} else if(0xe0 <= selector && selector <= 0xff) { // Negative Fixnum
					unpack_int8(*reinterpret_cast<const int8_t*>(m_current), obj);
					int ret = push_proc(obj, off);
					if (ret != 0) return ret;
				} else if(0xc0 <= selector && selector <= 0xdf) { // Variable
					switch(selector) {
					case 0xc0: {	// nil
						unpack_nil(obj);
						int ret = push_proc(obj, off);
						if (ret != 0) return ret;
					} break;
					//case 0xc1:  // string
					case 0xc2: {	// false
						unpack_false(obj);
						int ret = push_proc(obj, off);
						if (ret != 0) return ret;
					} break;
					case 0xc3: {	// true
						unpack_true(obj);
						int ret = push_proc(obj, off);
						if (ret != 0) return ret;
					} break;
					case 0xc4: // bin 8
					case 0xc5: // bin 16
					case 0xc6: // bin 32
						m_trail = 1 << (static_cast<unsigned int>(*m_current) & 0x03);
						m_cs = next_cs(m_current);
						fixed_trail_again = true;
						break;

					//case 0xc7:
					//case 0xc8:
					//case 0xc9:
					case 0xca:	// float
					case 0xcb:	// double
					case 0xcc:	// unsigned int	 8
					case 0xcd:	// unsigned int 16
					case 0xce:	// unsigned int 32
					case 0xcf:	// unsigned int 64
					case 0xd0:	// signed int  8
					case 0xd1:	// signed int 16
					case 0xd2:	// signed int 32
					case 0xd3:	// signed int 64
						m_trail = 1 << (static_cast<unsigned int>(*m_current) & 0x03);
						m_cs = next_cs(m_current);
						fixed_trail_again = true;
						break;
					//case 0xd4:
					//case 0xd5:
					//case 0xd6:  // big integer 16
					//case 0xd7:  // big integer 32
					//case 0xd8:  // big float 16
					case 0xd9:	// str 8
					case 0xda:	// str 16
					case 0xdb:	// str 32
						m_trail = 1 << ((static_cast<unsigned int>(*m_current) & 0x03) - 1);
						m_cs = next_cs(m_current);
						fixed_trail_again = true;
						break;
					case 0xdc:	// array 16
					case 0xdd:	// array 32
					case 0xde:	// map 16
					case 0xdf:	// map 32
						m_trail = 2 << (static_cast<unsigned int>(*m_current) & 0x01);
						m_cs = next_cs(m_current);
						fixed_trail_again = true;
						break;
					default:
						off = m_current - m_start;
						return -1;
					}
				} else if(0xa0 <= selector && selector <= 0xbf) { // FixStr
					m_trail = static_cast<unsigned int>(*m_current) & 0x1f;
					if(m_trail == 0) {
						unpack_str(m_user, data, n, m_trail, obj);
						int ret = push_proc(obj, off);
						if (ret != 0) return ret;
					}
					else {
						m_cs = ACS_STR_VALUE;
						fixed_trail_again = true;
					}

				} else if(0x90 <= selector && selector <= 0x9f) { // FixArray
					int ret = push_aggregate<fix_tag>(
						unpack_array(), CT_ARRAY_ITEM, obj, m_current, off);
					if (ret != 0) return ret;
				} else if(0x80 <= selector && selector <= 0x8f) { // FixMap
					int ret = push_aggregate<fix_tag>(
						unpack_map(), CT_MAP_KEY, obj, m_current, off);
					if (ret != 0) return ret;
				} else {
					off = m_current - m_start;
					return -1;
				}
				// end CS_HEADER
			}
			if (m_cs != CS_HEADER || fixed_trail_again) {
				if (fixed_trail_again) {
					++m_current;
					fixed_trail_again = false;
				}
				if((size_t)(pe - m_current) < m_trail) {
					off = m_current - m_start;
					return 0;
				}
				n = m_current;
				m_current += m_trail - 1;
				switch(m_cs) {
				//case CS_
				//case CS_
				case CS_FLOAT: {
					union { uint32_t i; float f; } mem;
					load<uint32_t>(mem.i, n);
					unpack_float(mem.f, obj);
					int ret = push_proc(obj, off);
					if (ret != 0) return ret;
				} break;
				case CS_DOUBLE: {
					union { uint64_t i; double f; } mem;
					load<uint64_t>(mem.i, n);
#if defined(__arm__) && !(__ARM_EABI__) // arm-oabi
					// https://github.com/msgpack/msgpack-perl/pull/1
					mem.i = (mem.i & 0xFFFFFFFFUL) << 32UL | (mem.i >> 32UL);
#endif
					unpack_double(mem.f, obj);
					int ret = push_proc(obj, off);
					if (ret != 0) return ret;
				} break;
				case CS_UINT_8: {
					uint8_t tmp;
					load<uint8_t>(tmp, n);
					unpack_uint8(tmp, obj);
					int ret = push_proc(obj, off);
					if (ret != 0) return ret;
				} break;
				case CS_UINT_16: {
					uint16_t tmp;
					load<uint16_t>(tmp, n);
					unpack_uint16(tmp, obj);
					int ret = push_proc(obj, off);
					if (ret != 0) return ret;
				} break;
				case CS_UINT_32: {
					uint32_t tmp;
					load<uint32_t>(tmp, n);
					unpack_uint32(tmp, obj);
					int ret = push_proc(obj, off);
					if (ret != 0) return ret;
				} break;
				case CS_UINT_64: {
					uint64_t tmp;
					load<uint64_t>(tmp, n);
					unpack_uint64(tmp, obj);
					int ret = push_proc(obj, off);
					if (ret != 0) return ret;
				} break;
				case CS_INT_8: {
					int8_t tmp;
					load<int8_t>(tmp, n);
					unpack_int8(tmp, obj);
					int ret = push_proc(obj, off);
					if (ret != 0) return ret;
				} break;
				case CS_INT_16: {
					int16_t tmp;
					load<int16_t>(tmp, n);
					unpack_int16(tmp, obj);
					int ret = push_proc(obj, off);
					if (ret != 0) return ret;
				} break;
				case CS_INT_32: {
					int32_t tmp;
					load<int32_t>(tmp, n);
					unpack_int32(tmp, obj);
					int ret = push_proc(obj, off);
					if (ret != 0) return ret;
				} break;
				case CS_INT_64: {
					int64_t tmp;
					load<int64_t>(tmp, n);
					unpack_int64(tmp, obj);
					int ret = push_proc(obj, off);
					if (ret != 0) return ret;
				} break;
				case CS_STR_8: {
					uint8_t tmp;
					load<uint8_t>(tmp, n);
					m_trail = tmp;
					if(m_trail == 0) {
						unpack_str(m_user, data, n, m_trail, obj);
						int ret = push_proc(obj, off);
						if (ret != 0) return ret;
					}
					else {
						m_cs = ACS_STR_VALUE;
						fixed_trail_again = true;
					}
				} break;
				case CS_BIN_8: {
					uint8_t tmp;
					load<uint8_t>(tmp, n);
					m_trail = tmp;
					if(m_trail == 0) {
						unpack_bin(m_user, data, n, m_trail, obj);
						int ret = push_proc(obj, off);
						if (ret != 0) return ret;
					}
					else {
						m_cs = ACS_BIN_VALUE;
						fixed_trail_again = true;
					}
				} break;
				case CS_STR_16: {
					uint16_t tmp;
					load<uint16_t>(tmp, n);
					m_trail = tmp;
					if(m_trail == 0) {
						unpack_str(m_user, data, n, m_trail, obj);
						int ret = push_proc(obj, off);
						if (ret != 0) return ret;
					}
					else {
						m_cs = ACS_STR_VALUE;
						fixed_trail_again = true;
					}
				} break;
				case CS_BIN_16: {
					uint16_t tmp;
					load<uint16_t>(tmp, n);
					m_trail = tmp;
					if(m_trail == 0) {
						unpack_bin(m_user, data, n, m_trail, obj);
						int ret = push_proc(obj, off);
						if (ret != 0) return ret;
					}
					else {
						m_cs = ACS_BIN_VALUE;
						fixed_trail_again = true;
					}
				} break;
				case CS_STR_32:
					load<uint32_t>(m_trail, n);
					if(m_trail == 0) {
						unpack_str(m_user, data, n, m_trail, obj);
						int ret = push_proc(obj, off);
						if (ret != 0) return ret;
					}
					else {
						m_cs = ACS_STR_VALUE;
						fixed_trail_again = true;
					}
					break;
				case CS_BIN_32:
					load<uint32_t>(m_trail, n);
					if(m_trail == 0) {
						unpack_bin(m_user, data, n, m_trail, obj);
						int ret = push_proc(obj, off);
						if (ret != 0) return ret;
					}
					else {
						m_cs = ACS_BIN_VALUE;
						fixed_trail_again = true;
					}
					break;
				case ACS_STR_VALUE: {
					unpack_str(m_user, data, n, m_trail, obj);
					int ret = push_proc(obj, off);
					if (ret != 0) return ret;
				} break;
				case ACS_BIN_VALUE: {
					unpack_bin(m_user, data, n, m_trail, obj);
					int ret = push_proc(obj, off);
					if (ret != 0) return ret;
				} break;
				case CS_ARRAY_16: {
					int ret = push_aggregate<uint16_t>(
						unpack_array(), CT_ARRAY_ITEM, obj, n, off);
					if (ret != 0) return ret;
				} break;
				case CS_ARRAY_32: {
					/* FIXME security guard */
					int ret = push_aggregate<uint32_t>(
						unpack_array(), CT_ARRAY_ITEM, obj, n, off);
					if (ret != 0) return ret;
				} break;
				case CS_MAP_16: {
					int ret = push_aggregate<uint16_t>(
						unpack_map(), CT_MAP_KEY, obj, n, off);
					if (ret != 0) return ret;
				} break;
				case CS_MAP_32: {
					/* FIXME security guard */
					int ret = push_aggregate<uint32_t>(
						unpack_map(), CT_MAP_KEY, obj, n, off);
					if (ret != 0) return ret;
				} break;
				default:
					off = m_current - m_start;
					return -1;
				}
			}
		} while(m_current != pe);

		off = m_current - m_start;
		return 0;
	}

private:
	template <typename T>
	static unsigned int next_cs(T p)
	{
		return static_cast<unsigned int>(*p) & 0x1f;
	}

	template <typename T, typename Func>
	int push_aggregate(
		Func const& f,
		unsigned int container_type,
		object& obj,
		const char* load_pos,
		size_t& off) {
		if(m_top < MSGPACK_EMBED_STACK_SIZE /* FIXME */) {
			typename value<T>::type tmp;
			load<T>(tmp, load_pos);
			if (f(m_user, tmp, m_stack[m_top].obj())) {
				if(tmp == 0) {
					obj = m_stack[m_top].obj();
					int ret = push_proc(obj, off);
					if (ret != 0) return ret;
				}
				else {
					m_stack[m_top].set_container_type(container_type);
					m_stack[m_top].set_count(tmp);
					++m_top;
					m_cs = CS_HEADER;
					++m_current;
				}
			}
			else {
				off = m_current - m_start;
				return -1;
			}
		}
		else {
			off = m_current - m_start;
			return -1;
		}
		return 0;
	}

	int push_item(object& obj) {
		bool finish = false;
		while (!finish) {
			if(m_top == 0) {
				return 1;
			}
			m_stack_idx = m_top - 1;
			unpack_stack* sp = &m_stack[m_stack_idx];
			switch(sp->container_type()) {
			case CT_ARRAY_ITEM:
				unpack_array_item(sp->obj(), obj);
				if(sp->decl_count() == 0) {
					obj = sp->obj();
					--m_top;
					/*printf("stack pop %d\n", m_top);*/
				}
				else {
					finish = true;
				}
				break;
			case CT_MAP_KEY:
				sp->set_map_key(obj);
				sp->set_container_type(CT_MAP_VALUE);
				finish = true;
				break;
			case CT_MAP_VALUE:
				unpack_map_item(sp->obj(), sp->map_key(), obj);
				if(sp->decl_count() == 0) {
					obj = sp->obj();
					--m_top;
					/*printf("stack pop %d\n", m_top);*/
				}
				else {
					sp->set_container_type(CT_MAP_KEY);
					finish = true;
				}
				break;
			default:
				return -1;
			}
		}
		return 0;
	}

	int push_proc(object& obj, size_t& off) {
		int ret = push_item(obj);
		if (ret > 0) {
			m_stack[0].set_obj(obj);
			++m_current;
			/*printf("-- finish --\n"); */
			off = m_current - m_start;
		}
		else if (ret < 0) {
			off = m_current - m_start;
		}
		else {
			m_cs = CS_HEADER;
			++m_current;
		}
		return ret;
	}

private:
	char const* m_start;
	char const* m_current;

	unsigned int m_trail;
	unpack_user m_user;
	unsigned int m_cs;
	unsigned int m_top;
	unsigned int m_stack_idx;
	unpack_stack m_stack[MSGPACK_EMBED_STACK_SIZE];
};

} // detail


struct unpack_error : public std::runtime_error {
	unpack_error(const std::string& msg) :
		std::runtime_error(msg) { }
};


class unpacked {
public:
	unpacked() { }

	unpacked(object const& obj, msgpack::unique_ptr<msgpack::zone> z) :
		m_obj(obj), m_zone(msgpack::move(z)) { }

	void set(object const& obj)
		{ m_obj = obj; }

	const object& get() const
		{ return m_obj; }

	msgpack::unique_ptr<msgpack::zone>& zone()
		{ return m_zone; }

	const msgpack::unique_ptr<msgpack::zone>& zone() const
		{ return m_zone; }

private:
	object m_obj;
	msgpack::unique_ptr<msgpack::zone> m_zone;
};


class unpacker {
public:
	unpacker(size_t init_buffer_size = MSGPACK_UNPACKER_INIT_BUFFER_SIZE);
	~unpacker();

public:
	/*! 1. reserve buffer. at least `size' bytes of capacity will be ready */
	void reserve_buffer(size_t size = MSGPACK_UNPACKER_RESERVE_SIZE);

	/*! 2. read data to the buffer() up to buffer_capacity() bytes */
	char* buffer();
	size_t buffer_capacity() const;

	/*! 3. specify the number of bytes actually copied */
	void buffer_consumed(size_t size);

	/*! 4. repeat next() until it retunrs false */
	bool next(unpacked* result);

	/*! 5. check if the size of message doesn't exceed assumption. */
	size_t message_size() const;

	// Basic usage of the unpacker is as following:
	//
	// unpacker pac;
	// while( /* input is readable */ ) {
	//
	//     // 1.
	//     pac.reserve_buffer(32*1024);
	//
	//     // 2.
	//     size_t bytes = input.readsome(pac.buffer(), pac.buffer_capacity());
	//
	//     // error handling ...
	//
	//     // 3.
	//     pac.buffer_consumed(bytes);
	//
	//     // 4.
	//     unpacked result;
	//     while(pac.next(&result)) {
	//         // do some with the object with the zone.
	//         object obj = result.get();
	//         std::auto_ptr<msgpack:zone> z = result.zone();
	//         on_message(obj, z);
	//
	//         //// boost::shared_ptr is also usable:
	//         // boost::shared_ptr<zone> life(z.release());
	//         // on_message(result.get(), life);
	//     }
	//
	//     // 5.
	//     if(pac.message_size() > 10*1024*1024) {
	//         throw std::runtime_error("message is too large");
	//     }
	// }
	//

	/*! for backward compatibility */
	bool execute();

	/*! for backward compatibility */
	object const& data();

	/*! for backward compatibility */
	zone* release_zone();

	/*! for backward compatibility */
	void reset_zone();

	/*! for backward compatibility */
	void reset();

public:
	// These functions are usable when non-MessagePack message follows after
	// MessagePack message.
	size_t parsed_size() const;

	/*! get address of the buffer that is not parsed */
	char* nonparsed_buffer();
	size_t nonparsed_size() const;

	/*! skip specified size of non-parsed buffer, leaving the buffer */
	// Note that the `size' argument must be smaller than nonparsed_size()
	void skip_nonparsed_buffer(size_t size);

	/*! remove unparsed buffer from unpacker */
	// Note that reset() leaves non-parsed buffer.
	void remove_nonparsed_buffer();

private:
	void expand_buffer(size_t size);
	int execute_imp();
	bool flush_zone();

private:
	char* m_buffer;
	size_t m_used;
	size_t m_free;
	size_t m_off;
	size_t m_parsed;
	zone* m_z;
	size_t m_initial_buffer_size;
	detail::context m_ctx;

private:
	unpacker(const unpacker&);
};


inline void unpack(unpacked& result,
		const char* data, size_t len, size_t* offset = nullptr);
inline void unpack(unpacked* result,
		const char* data, size_t len, size_t* offset = nullptr);

// obsolete
typedef enum {
	UNPACK_SUCCESS				=  2,
	UNPACK_EXTRA_BYTES			=  1,
	UNPACK_CONTINUE			=  0,
	UNPACK_PARSE_ERROR			= -1
} unpack_return;

// obsolete
static unpack_return unpack(const char* data, size_t len, size_t* off,
		zone& z, object& result);
static unpack_return unpack(const char* data, size_t len, size_t* off,
		zone* z, object* result);


// obsolete
static object unpack(const char* data, size_t len, zone& z, size_t* off = nullptr);
static object unpack(const char* data, size_t len, zone* z, size_t* off = nullptr);


inline unpacker::unpacker(size_t initial_buffer_size)
{
	if(initial_buffer_size < COUNTER_SIZE) {
		initial_buffer_size = COUNTER_SIZE;
	}

	char* buffer = static_cast<char*>(::malloc(initial_buffer_size));
	if(!buffer) {
		throw std::bad_alloc();
	}

	zone* z = zone::create(MSGPACK_ZONE_CHUNK_SIZE);
	if(!z) {
		::free(buffer);
		throw std::bad_alloc();
	}

	m_buffer = buffer;
	m_used = COUNTER_SIZE;
	m_free = initial_buffer_size - m_used;
	m_off = COUNTER_SIZE;
	m_parsed = 0;
	m_initial_buffer_size = initial_buffer_size;
	m_z = z;

	detail::init_count(m_buffer);

	m_ctx.init();
	m_ctx.user().set_zone(*m_z);
	m_ctx.user().set_referenced(false);
}

inline unpacker::~unpacker()
{
	zone::destroy(m_z);
	detail::decl_count(m_buffer);
}


inline void unpacker::reserve_buffer(size_t size)
{
	if(m_free >= size) return;
	expand_buffer(size);
}

inline void unpacker::expand_buffer(size_t size)
{
	if(m_used == m_off && detail::get_count(m_buffer) == 1
		&& !m_ctx.user().referenced()) {
		// rewind buffer
		m_free += m_used - COUNTER_SIZE;
		m_used = COUNTER_SIZE;
		m_off  = COUNTER_SIZE;

		if(m_free >= size) return;
	}

	if(m_off == COUNTER_SIZE) {
		size_t next_size = (m_used + m_free) * 2;	 // include COUNTER_SIZE
		while(next_size < size + m_used) {
			next_size *= 2;
		}

		char* tmp = static_cast<char*>(::realloc(m_buffer, next_size));
		if(!tmp) {
			throw std::bad_alloc();
		}

		m_buffer = tmp;
		m_free = next_size - m_used;

	} else {
		size_t next_size = m_initial_buffer_size;  // include COUNTER_SIZE
		size_t not_parsed = m_used - m_off;
		while(next_size < size + not_parsed + COUNTER_SIZE) {
			next_size *= 2;
		}

		char* tmp = static_cast<char*>(::malloc(next_size));
		if(!tmp) {
			throw std::bad_alloc();
		}

		detail::init_count(tmp);

		::memcpy(tmp+COUNTER_SIZE, m_buffer + m_off, not_parsed);

		if(m_ctx.user().referenced()) {
			try {
				m_z->push_finalizer(&detail::decl_count, m_buffer);
			}
			catch (...) {
				::free(tmp);
				throw;
			}
			m_ctx.user().set_referenced(false);
		} else {
			detail::decl_count(m_buffer);
		}

		m_buffer = tmp;
		m_used	= not_parsed + COUNTER_SIZE;
		m_free	= next_size - m_used;
		m_off	= COUNTER_SIZE;
	}
}

inline char* unpacker::buffer()
{
	return m_buffer + m_used;
}

inline size_t unpacker::buffer_capacity() const
{
	return m_free;
}

inline void unpacker::buffer_consumed(size_t size)
{
	m_used += size;
	m_free -= size;
}

inline bool unpacker::next(unpacked* result)
{
	int ret = execute_imp();

	if(ret < 0) {
		throw unpack_error("parse error");
	}

	if(ret == 0) {
		result->zone().reset();
		result->set(object());
		return false;

	} else {
		result->zone().reset( release_zone() );
		result->set(data());
		reset();
		return true;
	}
}


inline bool unpacker::execute()
{
	int ret = execute_imp();
	if(ret < 0) {
		throw unpack_error("parse error");
	} else if(ret == 0) {
		return false;
	} else {
		return true;
	}
}

inline int unpacker::execute_imp()
{
	size_t off = m_off;
	int ret = m_ctx.execute(m_buffer, m_used, m_off);
	if(m_off > off) {
		m_parsed += m_off - off;
	}
	return ret;
}

inline object const& unpacker::data()
{
	return m_ctx.data();
}

inline zone* unpacker::release_zone()
{
	if(!flush_zone()) {
		return nullptr;
	}

	zone* r =  zone::create(MSGPACK_ZONE_CHUNK_SIZE);
	if(!r) {
		return nullptr;
	}

	zone* old = m_z;
	m_z = r;
	m_ctx.user().set_zone(*m_z);

	return old;
}

inline void unpacker::reset_zone()
{
	m_z->clear();
}

inline bool unpacker::flush_zone()
{
	if(m_ctx.user().referenced()) {
		try {
			m_z->push_finalizer(&detail::decl_count, m_buffer);
		} catch (...) {
			return false;
		}
		m_ctx.user().set_referenced(false);

		detail::incr_count(m_buffer);
	}

	return true;
}

inline void unpacker::reset()
{
	m_ctx.init();
	// don't reset referenced flag
	m_parsed = 0;
}

inline size_t unpacker::message_size() const
{
	return m_parsed - m_off + m_used;
}

inline size_t unpacker::parsed_size() const
{
	return m_parsed;
}

inline char* unpacker::nonparsed_buffer()
{
	return m_buffer + m_off;
}

inline size_t unpacker::nonparsed_size() const
{
	return m_used - m_off;
}

inline void unpacker::skip_nonparsed_buffer(size_t size)
{
	m_off += size;
}

inline void unpacker::remove_nonparsed_buffer()
{
	m_used = m_off;
}

namespace detail {

inline unpack_return
unpack_imp(const char* data, size_t len, size_t* off,
	zone& result_zone, object& result)
{
	size_t noff = 0;
	if(off != nullptr) { noff = *off; }

	if(len <= noff) {
		// FIXME
		return UNPACK_CONTINUE;
	}

	detail::context ctx;
	ctx.init();

	ctx.user().set_zone(result_zone);
	ctx.user().set_referenced(false);

	int e = ctx.execute(data, len, noff);
	if(e < 0) {
		return UNPACK_PARSE_ERROR;
	}

	if(off != nullptr) { *off = noff; }

	if(e == 0) {
		return UNPACK_CONTINUE;
	}

	result = ctx.data();

	if(noff < len) {
		return UNPACK_EXTRA_BYTES;
	}

	return UNPACK_SUCCESS;
}

} // detail

// reference version
inline void unpack(unpacked& result,
	const char* data, size_t len, size_t* offset)
{
	object obj;
	msgpack::unique_ptr<zone> z(new zone());

	unpack_return ret = detail::unpack_imp(
			data, len, offset, *z, obj);


	switch(ret) {
	case UNPACK_SUCCESS:
		result.set(obj);
		result.zone() = msgpack::move(z);
		return;

	case UNPACK_EXTRA_BYTES:
		result.set(obj);
		result.zone() = msgpack::move(z);
		return;

	case UNPACK_CONTINUE:
		throw unpack_error("insufficient bytes");

	case UNPACK_PARSE_ERROR:
	default:
		throw unpack_error("parse error");
	}
}
// pointer version
inline void unpack(unpacked* result,
	const char* data, size_t len, size_t* offset) {
	unpack(*result, data, len, offset);
}


// obsolete
// reference version
inline unpack_return unpack(const char* data, size_t len, size_t* off,
		zone& z, object& result)
{
	return detail::unpack_imp(data, len, off,
			z, result);
}
// pointer version
inline unpack_return unpack(const char* data, size_t len, size_t* off,
		zone* z, object* result)
{
	return unpack(data, len, off, *z, *result);
}

// obsolete
// reference version
inline object unpack(const char* data, size_t len, zone& z, size_t* off)
{
	object result;

	switch( unpack(data, len, off, z, result) ) {
	case UNPACK_SUCCESS:
		return result;

	case UNPACK_EXTRA_BYTES:
		if(off) {
			return result;
		} else {
			throw unpack_error("extra bytes");
		}

	case UNPACK_CONTINUE:
		throw unpack_error("insufficient bytes");

	case UNPACK_PARSE_ERROR:
	default:
		throw unpack_error("parse error");
	}
}
// pointer version
inline object unpack(const char* data, size_t len, zone* z, size_t* off)
{
	return unpack(data, len, *z, off);
}

}  // namespace msgpack

#endif /* msgpack/unpack.hpp */

