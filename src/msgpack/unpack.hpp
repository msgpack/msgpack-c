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
	zone const& z() const { return *z_; }
	zone& z() { return *z_; }
	void set_z(zone& z) { z_ = &z; }
	bool referenced() const { return referenced_; }
	void set_referenced(bool referenced) { referenced_ = referenced; }
private:
	zone* z_;
	bool referenced_;
};

inline void unpack_uint8(unpack_user&, uint8_t d, object& o)
{ o.type = type::POSITIVE_INTEGER; o.via.u64 = d; }

inline void unpack_uint16(unpack_user&, uint16_t d, object& o)
{ o.type = type::POSITIVE_INTEGER; o.via.u64 = d; }

inline void unpack_uint32(unpack_user&, uint32_t d, object& o)
{ o.type = type::POSITIVE_INTEGER; o.via.u64 = d; }

inline void unpack_uint64(unpack_user&, uint64_t d, object& o)
{ o.type = type::POSITIVE_INTEGER; o.via.u64 = d; }

inline void unpack_int8(unpack_user&, int8_t d, object& o)
{ if(d >= 0) { o.type = type::POSITIVE_INTEGER; o.via.u64 = d; }
		else { o.type = type::NEGATIVE_INTEGER; o.via.i64 = d; } }

inline void unpack_int16(unpack_user&, int16_t d, object& o)
{ if(d >= 0) { o.type = type::POSITIVE_INTEGER; o.via.u64 = d; }
		else { o.type = type::NEGATIVE_INTEGER; o.via.i64 = d; } }

inline void unpack_int32(unpack_user&, int32_t d, object& o)
{ if(d >= 0) { o.type = type::POSITIVE_INTEGER; o.via.u64 = d; }
		else { o.type = type::NEGATIVE_INTEGER; o.via.i64 = d; } }

inline void unpack_int64(unpack_user&, int64_t d, object& o)
{ if(d >= 0) { o.type = type::POSITIVE_INTEGER; o.via.u64 = d; }
		else { o.type = type::NEGATIVE_INTEGER; o.via.i64 = d; } }

inline void unpack_float(unpack_user&, float d, object& o)
{ o.type = type::DOUBLE; o.via.dec = d; }

inline void unpack_double(unpack_user&, double d, object& o)
{ o.type = type::DOUBLE; o.via.dec = d; }

inline void unpack_nil(unpack_user&, object& o)
{ o.type = type::NIL; }

inline void unpack_true(unpack_user&, object& o)
{ o.type = type::BOOLEAN; o.via.boolean = true; }

inline void unpack_false(unpack_user&, object& o)
{ o.type = type::BOOLEAN; o.via.boolean = false; }

struct unpack_array {
	bool operator()(unpack_user&u, unsigned int n, object& o) const {
		o.type = type::ARRAY;
		o.via.array.size = 0;
		o.via.array.ptr = (object*)u.z().malloc(n*sizeof(object));
		if(o.via.array.ptr == NULL) { return false; }
		return true;
	}
};

inline void unpack_array_item(unpack_user&, object& c, object const& o)
{ c.via.array.ptr[c.via.array.size++] = o; }

struct unpack_map {
	bool operator()(unpack_user& u, unsigned int n, object& o) const {
		o.type = type::MAP;
		o.via.map.size = 0;
		o.via.map.ptr = (object_kv*)u.z().malloc(n*sizeof(object_kv));
		if(o.via.map.ptr == NULL) { return false; }
		return true;
	}
};

inline void unpack_map_item(unpack_user&, object& c, object const& k, object const& v)
{
	c.via.map.ptr[c.via.map.size].key = k;
	c.via.map.ptr[c.via.map.size].val = v;
	++c.via.map.size;
}

inline void unpack_raw(unpack_user& u, const char* b, const char* p, unsigned int l, object& o)
{
	o.type = type::RAW;
	o.via.raw.ptr = p;
	o.via.raw.size = l;
	u.set_referenced(true);
}


class template_unpack_stack {
public:
	object const& obj() const { return obj_; }
	object& obj() { return obj_; }
	void setObj(object const& obj) { obj_ = obj; }
	size_t count() const { return count_; }
	void set_count(size_t count) { count_ = count; }
	size_t decl_count() { return --count_; }
	unsigned int ct() const { return ct_; }
	void set_ct(unsigned int ct) { ct_ = ct; }
	object const& map_key() const { return map_key_; }
	void set_map_key(object const& map_key) { map_key_ = map_key; }
private:
	object obj_;
	size_t count_;
	unsigned int ct_;
	object map_key_;
};

inline void init_count(void* buffer)
{
	*(volatile _msgpack_atomic_counter_t*)buffer = 1;
}

inline void decl_count(void* buffer)
{
	// atomic if(--*(_msgpack_atomic_counter_t*)buffer == 0) { free(buffer); }
	if(_msgpack_sync_decr_and_fetch((volatile _msgpack_atomic_counter_t*)buffer) == 0) {
		free(buffer);
	}
}

inline void incr_count(void* buffer)
{
	// atomic ++*(_msgpack_atomic_counter_t*)buffer;
	_msgpack_sync_incr_and_fetch((volatile _msgpack_atomic_counter_t*)buffer);
}

inline _msgpack_atomic_counter_t get_count(void* buffer)
{
	return *(volatile _msgpack_atomic_counter_t*)buffer;
}

struct fix_tag {
	char f1[65]; // FIXME unique size is required. or use is_same meta function.
};

template <typename T>
inline unsigned int load(const char* n, typename msgpack::enable_if<sizeof(T) == sizeof(fix_tag)>::type* = nullptr) {
	return static_cast<unsigned int>(*reinterpret_cast<const uint8_t*>(n)) & 0x0f;
}

template <typename T>
inline T load(const char* n, typename msgpack::enable_if<sizeof(T) == 1>::type* = nullptr) {
	return static_cast<T>(*reinterpret_cast<const uint8_t*>(n));
}

template <typename T>
inline T load(const char* n, typename msgpack::enable_if<sizeof(T) == 2>::type* = nullptr) {
	return static_cast<T>(
		(static_cast<uint16_t>(reinterpret_cast<const uint8_t*>(n)[0]) <<  8) |
		(static_cast<uint16_t>(reinterpret_cast<const uint8_t*>(n)[1])      ));
}

template <typename T>
inline T load(const char* n, typename msgpack::enable_if<sizeof(T) == 4>::type* = nullptr) {
	return static_cast<T>(
		(static_cast<uint32_t>(reinterpret_cast<const uint8_t*>(n)[0]) << 24) |
		(static_cast<uint32_t>(reinterpret_cast<const uint8_t*>(n)[1]) << 16) |
		(static_cast<uint32_t>(reinterpret_cast<const uint8_t*>(n)[2]) <<  8) |
		(static_cast<uint32_t>(reinterpret_cast<const uint8_t*>(n)[3])      ));
}

template <typename T>
inline T load(const char* n, typename msgpack::enable_if<sizeof(T) == 8>::type* = nullptr) {
	return static_cast<T>(
		(static_cast<uint64_t>(reinterpret_cast<const uint8_t*>(n)[0]) << 56) |
		(static_cast<uint64_t>(reinterpret_cast<const uint8_t*>(n)[1]) << 48) |
		(static_cast<uint64_t>(reinterpret_cast<const uint8_t*>(n)[2]) << 40) |
		(static_cast<uint64_t>(reinterpret_cast<const uint8_t*>(n)[3]) << 32) |
		(static_cast<uint64_t>(reinterpret_cast<const uint8_t*>(n)[4]) << 24) |
		(static_cast<uint64_t>(reinterpret_cast<const uint8_t*>(n)[5]) << 16) |
		(static_cast<uint64_t>(reinterpret_cast<const uint8_t*>(n)[6]) <<  8) |
		(static_cast<uint64_t>(reinterpret_cast<const uint8_t*>(n)[7])      ));
}

class template_context {
public:
	template_context():trail_(0), cs_(CS_HEADER), top_(0)
	{
		stack_[0].setObj(object());
	}

	void init()
	{
		cs_ = CS_HEADER;
		trail_ = 0;
		top_ = 0;
		stack_[0].setObj(object());
	}

	object const& data() const
	{
		return stack_[0].obj();
	}

	unpack_user& user()
	{
		return user_;
	}

	unpack_user const& user() const
	{
		return user_;
	}

	int execute(const char* data, size_t len, size_t& off)
	{
		assert(len >= off);

		const char* p = data + off;
		const char* const pe = data + len;
		const char* n = nullptr;

		// to support register optimization
		unsigned int cs = cs_;
		unsigned int trail = trail_;
		unpack_user user = user_;
		unsigned int top = top_;
		template_unpack_stack* stack = stack_;

		object obj;
		template_unpack_stack* c = nullptr;

		if(p == pe) {
			off = update_attributes(p, data, top, cs, trail, user);
			return 0;
		}
		bool fixed_trail_again = false;
		do {
			if (cs == CS_HEADER) {
				fixed_trail_again = false;
				int selector = *reinterpret_cast<const unsigned char*>(p);
				if (0) {
				} else if(0x00 <= selector && selector <= 0x7f) { // Positive Fixnum
					unpack_uint8(user, *(uint8_t*)p, obj);
					int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
					if (ret != 0) return ret;
				} else if(0xe0 <= selector && selector <= 0xff) { // Negative Fixnum
					unpack_int8(user, *(int8_t*)p, obj);
					int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
					if (ret != 0) return ret;
				} else if(0xc0 <= selector && selector <= 0xdf) { // Variable
					switch(selector) {
					case 0xc0: {	// nil
						unpack_nil(user, obj);
						int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
						if (ret != 0) return ret;
					} break;
					//case 0xc1:  // string
					case 0xc2: {	// false
						unpack_false(user, obj);
						int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
						if (ret != 0) return ret;
					} break;
					case 0xc3: {	// true
						unpack_true(user, obj);
						int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
						if (ret != 0) return ret;
					} break;
					case 0xc4: // bin 8
					case 0xc5: // bin 16
					case 0xc6: // bin 32
						trail = 1 << (((unsigned int)*p) & 0x03);
						cs = next_cs(p);
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
						trail = 1 << (((unsigned int)*p) & 0x03);
						cs = next_cs(p);
						fixed_trail_again = true;
						break;
					//case 0xd4:
					//case 0xd5:
					//case 0xd6:  // big integer 16
					//case 0xd7:  // big integer 32
					//case 0xd8:  // big float 16
					case 0xd9:	// raw 8 (str 8)
					case 0xda:	// raw 16 (str 16)
					case 0xdb:	// raw 32 (str 32)
						trail = 1 << ((((unsigned int)*p) & 0x03) - 1);
						cs = next_cs(p);
						fixed_trail_again = true;
						break;
					case 0xdc:	// array 16
					case 0xdd:	// array 32
					case 0xde:	// map 16
					case 0xdf:	// map 32
						trail = 2 << (((unsigned int)*p) & 0x01);
						cs = next_cs(p);
						fixed_trail_again = true;
						break;
					default:
						off = update_attributes(p, data, top, cs, trail, user);
						return -1;
					}
				} else if(0xa0 <= selector && selector <= 0xbf) { // FixRaw
					trail = (unsigned int)*p & 0x1f;
					if(trail == 0) {
						unpack_raw(user, data, n, trail, obj);
						int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
						if (ret != 0) return ret;
					}
					cs = ACS_RAW_VALUE;
					fixed_trail_again = true;

				} else if(0x90 <= selector && selector <= 0x9f) { // FixArray
					int ret = push_aggregate<fix_tag>(
						unpack_array(), CT_ARRAY_ITEM, stack, c, obj, p, p, data, off, top, cs, trail, user);
					if (ret != 0) return ret;
				} else if(0x80 <= selector && selector <= 0x8f) { // FixMap
					int ret = push_aggregate<fix_tag>(
						unpack_map(), CT_MAP_KEY, stack, c, obj, p, p, data, off, top, cs, trail, user);
					if (ret != 0) return ret;
				} else {
					off = update_attributes(p, data, top, cs, trail, user);
					return -1;
				}
				// end CS_HEADER
			}
			if (cs != CS_HEADER || fixed_trail_again) {
				if (fixed_trail_again) {
					++p;
					fixed_trail_again = false;
				}
				if((size_t)(pe - p) < trail) {
					off = update_attributes(p, data, top, cs, trail, user);
					return 0;
				}
				n = p;
				p += trail - 1;
				switch(cs) {
				//case CS_
				//case CS_
				case CS_FLOAT: {
					union { uint32_t i; float f; } mem;
					mem.i = load<uint32_t>(n);
					unpack_float(user, mem.f, obj);
					int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
					if (ret != 0) return ret;
				} break;
				case CS_DOUBLE: {
					union { uint64_t i; double f; } mem;
					mem.i = load<uint64_t>(n);
#if defined(__arm__) && !(__ARM_EABI__) // arm-oabi
					// https://github.com/msgpack/msgpack-perl/pull/1
					mem.i = (mem.i & 0xFFFFFFFFUL) << 32UL | (mem.i >> 32UL);
#endif
					unpack_double(user, mem.f, obj);
					int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
					if (ret != 0) return ret;
				} break;
				case CS_UINT_8: {
					unpack_uint8(user, load<uint8_t>(n), obj);
					int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
					if (ret != 0) return ret;
				} break;
				case CS_UINT_16: {
					unpack_uint16(user, load<uint16_t>(n), obj);
					int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
					if (ret != 0) return ret;
				} break;
				case CS_UINT_32: {
					unpack_uint32(user, load<uint32_t>(n), obj);
					int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
					if (ret != 0) return ret;
				} break;
				case CS_UINT_64: {
					unpack_uint64(user, load<uint64_t>(n), obj);
					int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
					if (ret != 0) return ret;
				} break;
				case CS_INT_8: {
					unpack_int8(user, load<uint8_t>(n), obj);
					int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
					if (ret != 0) return ret;
				} break;
				case CS_INT_16: {
					unpack_int16(user, load<int16_t>(n), obj);
					int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
					if (ret != 0) return ret;
				} break;
				case CS_INT_32: {
					unpack_int32(user, load<int32_t>(n), obj);
					int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
					if (ret != 0) return ret;
				} break;
				case CS_INT_64: {
					unpack_int64(user, load<int64_t>(n), obj);
					int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
					if (ret != 0) return ret;
				} break;
				case CS_BIN_8:
				case CS_RAW_8:
					trail = load<uint8_t>(n);
					if(trail == 0) {
						unpack_raw(user, data, n, trail, obj);
						int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
						if (ret != 0) return ret;
					}
					else {
						cs = ACS_RAW_VALUE;
						fixed_trail_again = true;
					}
					break;
				case CS_BIN_16:
				case CS_RAW_16:
					trail = load<uint16_t>(n);
					if(trail == 0) {
						unpack_raw(user, data, n, trail, obj);
						int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
						if (ret != 0) return ret;
					}
					else {
						cs = ACS_RAW_VALUE;
						fixed_trail_again = true;
					}
					break;
				case CS_BIN_32:
				case CS_RAW_32:
					trail = load<uint32_t>(n);
					if(trail == 0) {
						unpack_raw(user, data, n, trail, obj);
						int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
						if (ret != 0) return ret;
					}
					else {
						cs = ACS_RAW_VALUE;
						fixed_trail_again = true;
					}
					break;
				case ACS_RAW_VALUE: {
					unpack_raw(user, data, n, trail, obj);
					int ret = push_proc(stack, c, obj, p, data, off, top, cs, trail, user);
					if (ret != 0) return ret;
				} break;
				case CS_ARRAY_16: {
					int ret = push_aggregate<uint16_t>(
						unpack_array(), CT_ARRAY_ITEM, stack, c, obj, p, n, data, off, top, cs, trail, user);
					if (ret != 0) return ret;
				} break;
				case CS_ARRAY_32: {
					/* FIXME security guard */
					int ret = push_aggregate<uint32_t>(
						unpack_array(), CT_ARRAY_ITEM, stack, c, obj, p, n, data, off, top, cs, trail, user);
					if (ret != 0) return ret;
				} break;
				case CS_MAP_16: {
					int ret = push_aggregate<uint16_t>(
						unpack_map(), CT_MAP_KEY, stack, c, obj, p, n, data, off, top, cs, trail, user);
					if (ret != 0) return ret;
				} break;
				case CS_MAP_32: {
					/* FIXME security guard */
					int ret = push_aggregate<uint32_t>(
						unpack_map(), CT_MAP_KEY, stack, c, obj, p, n, data, off, top, cs, trail, user);
					if (ret != 0) return ret;
				} break;
				default:
					off = update_attributes(p, data, top, cs, trail, user);
					return -1;
				}
			}
		} while(p != pe);

		off = update_attributes(p, data, top, cs, trail, user);
		return 0;
	}

private:
	template <typename T>
	static unsigned int next_cs(T p)
	{
		return (unsigned int)*p & 0x1f;
	}

	template <typename T, typename Func>
	int push_aggregate(
		Func const& f,
		unsigned int ct,
		template_unpack_stack* stack,
		template_unpack_stack*& c,
		object& obj,
		const char*& current,
		const char* load_pos,
		const char* origin,
		size_t& off,
		unsigned int& top,
		unsigned int& cs,
		unsigned int trail,
		unpack_user& user) {
		if(top < MSGPACK_EMBED_STACK_SIZE /* FIXME */
		   && f(user, load<T>(load_pos), stack[top].obj())) {
			if(load<T>(load_pos) == 0) {
				obj = stack[top].obj();
				int ret = push_proc(stack, c, obj, current, origin, off, top, cs, trail, user);
				if (ret != 0) return ret;
			}
			else {
				stack[top].set_ct(ct);
				stack[top].set_count(load<T>(load_pos));
				++top;
				header_again(cs, current);
			}
		}
		else {
			off = update_attributes(current, origin, top, cs, trail, user);
			return -1;
		}
		return 0;
	}

	size_t update_attributes(
		const char* current,
		const char* origin,
		unsigned int top,
		unsigned int cs,
		unsigned int trail,
		unpack_user const& user) {
		trail_ = trail;
		top_ = top;
		cs_ = cs;
		user_ = user;
		return current - origin;
	}

	static void header_again(unsigned int& cs, const char*& current) {
		cs = CS_HEADER;
		++current;
	}
	static int push_item(
		template_unpack_stack* stack,
		template_unpack_stack*& c,
		object& obj,
		unsigned int top,
		unpack_user& user) {
		bool finish = false;
		while (!finish) {
			if(top == 0) {
				return 1;
			}
			c = &stack[top-1];
			switch(c->ct()) {
			case CT_ARRAY_ITEM:
				unpack_array_item(user, c->obj(), obj);
				if(c->decl_count() == 0) {
					obj = c->obj();
					--top;
					/*printf("stack pop %d\n", top);*/
				}
				else {
					finish = true;
				}
				break;
			case CT_MAP_KEY:
				c->set_map_key(obj);
				c->set_ct(CT_MAP_VALUE);
				finish = true;
				break;
			case CT_MAP_VALUE:
				unpack_map_item(user, c->obj(), c->map_key(), obj);
				if(c->decl_count() == 0) {
					obj = c->obj();
					--top;
					/*printf("stack pop %d\n", top);*/
				}
				else {
					c->set_ct(CT_MAP_KEY);
					finish = true;
				}
				break;
			default:
				return -1;
			}
		}
		return 0;
	}

	int push_proc(
		template_unpack_stack* stack,
		template_unpack_stack*& c,
		object& obj,
		const char*& current,
		const char* origin,
		size_t& off,
		unsigned int top,
		unsigned int& cs,
		unsigned int trail,
		unpack_user& user) {
		int ret = push_item(stack, c, obj, top, user);
		if (ret > 0) {
			stack[0].setObj(obj);
			++current;
			/*printf("-- finish --\n"); */
			off = update_attributes(current, origin, top, cs, trail, user);
		}
		else if (ret < 0) {
			off = update_attributes(current, origin, top, cs, trail, user);
		}
		else {
			header_again(cs, current);
		}
		return ret;
	}

private:
	unsigned int trail_;
	unpack_user user_;
	unsigned int cs_;
	unsigned int top_;
	template_unpack_stack stack_[MSGPACK_EMBED_STACK_SIZE];
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
	char* buffer_;
	size_t used_;
	size_t free_;
	size_t off_;
	size_t parsed_;
	zone* z_;
	size_t initial_buffer_size_;
	detail::template_context ctx_;

private:
	unpacker(const unpacker&);
};


inline void unpack(unpacked& result,
		const char* data, size_t len, size_t* offset = NULL);
inline void unpack(unpacked* result,
		const char* data, size_t len, size_t* offset = NULL);

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
static object unpack(const char* data, size_t len, zone& z, size_t* off = NULL);
static object unpack(const char* data, size_t len, zone* z, size_t* off = NULL);


inline unpacker::unpacker(size_t initial_buffer_size)
{
	if(initial_buffer_size < COUNTER_SIZE) {
		initial_buffer_size = COUNTER_SIZE;
	}

	char* buffer = (char*)::malloc(initial_buffer_size);
	if(!buffer) {
		throw std::bad_alloc();
	}

	zone* z = zone::create(MSGPACK_ZONE_CHUNK_SIZE);
	if(!z) {
		::free(buffer);
		throw std::bad_alloc();
	}

	buffer_ = buffer;
	used_ = COUNTER_SIZE;
	free_ = initial_buffer_size - used_;
	off_ = COUNTER_SIZE;
	parsed_ = 0;
	initial_buffer_size_ = initial_buffer_size;
	z_ = z;

	detail::init_count(buffer_);

	ctx_.init();
	ctx_.user().set_z(*z_);
	ctx_.user().set_referenced(false);
}

inline unpacker::~unpacker()
{
	zone::destroy(z_);
	detail::decl_count(buffer_);
}


inline void unpacker::reserve_buffer(size_t size)
{
	if(free_ >= size) return;
	expand_buffer(size);
}

inline void unpacker::expand_buffer(size_t size)
{
	if(used_ == off_ && detail::get_count(buffer_) == 1
		&& !ctx_.user().referenced()) {
		// rewind buffer
		free_ += used_ - COUNTER_SIZE;
		used_ = COUNTER_SIZE;
		off_  = COUNTER_SIZE;

		if(free_ >= size) return;
	}

	if(off_ == COUNTER_SIZE) {
		size_t next_size = (used_ + free_) * 2;	 // include COUNTER_SIZE
		while(next_size < size + used_) {
			next_size *= 2;
		}

		char* tmp = (char*)::realloc(buffer_, next_size);
		if(!tmp) {
			throw std::bad_alloc();
		}

		buffer_ = tmp;
		free_ = next_size - used_;

	} else {
		size_t next_size = initial_buffer_size_;  // include COUNTER_SIZE
		size_t not_parsed = used_ - off_;
		while(next_size < size + not_parsed + COUNTER_SIZE) {
			next_size *= 2;
		}

		char* tmp = (char*)::malloc(next_size);
		if(!tmp) {
			throw std::bad_alloc();
		}

		detail::init_count(tmp);

		::memcpy(tmp+COUNTER_SIZE, buffer_ + off_, not_parsed);

		if(ctx_.user().referenced()) {
			try {
				z_->push_finalizer(&detail::decl_count, buffer_);
			}
			catch (...) {
				::free(tmp);
				throw;
			}
			ctx_.user().set_referenced(false);
		} else {
			detail::decl_count(buffer_);
		}

		buffer_ = tmp;
		used_	= not_parsed + COUNTER_SIZE;
		free_	= next_size - used_;
		off_	= COUNTER_SIZE;
	}
}

inline char* unpacker::buffer()
{
	return buffer_ + used_;
}

inline size_t unpacker::buffer_capacity() const
{
	return free_;
}

inline void unpacker::buffer_consumed(size_t size)
{
	used_ += size;
	free_ -= size;
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
	size_t off = off_;
	int ret = ctx_.execute(buffer_, used_, off_);
	if(off_ > off) {
		parsed_ += off_ - off;
	}
	return ret;
}

inline object const& unpacker::data()
{
	return ctx_.data();
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

	zone* old = z_;
	z_ = r;
	ctx_.user().set_z(*z_);

	return old;
}

inline void unpacker::reset_zone()
{
	z_->clear();
}

inline bool unpacker::flush_zone()
{
	if(ctx_.user().referenced()) {
		try {
			z_->push_finalizer(&detail::decl_count, buffer_);
		} catch (...) {
			return false;
		}
		ctx_.user().set_referenced(false);

		detail::incr_count(buffer_);
	}

	return true;
}

inline void unpacker::reset()
{
	ctx_.init();
	// don't reset referenced flag
	parsed_ = 0;
}

inline size_t unpacker::message_size() const
{
	return parsed_ - off_ + used_;
}

inline size_t unpacker::parsed_size() const
{
	return parsed_;
}

inline char* unpacker::nonparsed_buffer()
{
	return buffer_ + off_;
}

inline size_t unpacker::nonparsed_size() const
{
	return used_ - off_;
}

inline void unpacker::skip_nonparsed_buffer(size_t size)
{
	off_ += size;
}

inline void unpacker::remove_nonparsed_buffer()
{
	used_ = off_;
}

namespace detail {

inline unpack_return
unpack_imp(const char* data, size_t len, size_t* off,
	zone& result_zone, object& result)
{
	size_t noff = 0;
	if(off != NULL) { noff = *off; }

	if(len <= noff) {
		// FIXME
		return UNPACK_CONTINUE;
	}

	detail::template_context ctx;
	ctx.init();

	ctx.user().set_z(result_zone);
	ctx.user().set_referenced(false);

	int e = ctx.execute(data, len, noff);
	if(e < 0) {
		return UNPACK_PARSE_ERROR;
	}

	if(off != NULL) { *off = noff; }

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

