//
// MessagePack for C++ deserializing routine
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MSGPACK_V2_EMBED_STACK_HPP
#define MSGPACK_V2_EMBED_STACK_HPP

#include "msgpack/versioning.hpp"

#include <cstddef>
#include <vector>

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v2) {
/// @endcond

namespace detail {

// The parse stacks: the first N entries are stored inline, so that unpacking an object nested at most
// N levels deep does not allocate; deeper levels go to a vector.
template <typename T, std::size_t N>
class embed_stack {
public:
    embed_stack():m_size(0) {}
    void push_back(T const& v) {
        if (m_size < N) m_embed[m_size] = v;
        else m_more.push_back(v);
        ++m_size;
    }
    void pop_back() {
        --m_size;
        if (m_size >= N) m_more.pop_back();
    }
    T& back() { return (*this)[m_size - 1]; }
    T& operator[](std::size_t i) { return i < N ? m_embed[i] : m_more[i - N]; }
    std::size_t size() const { return m_size; }
    bool empty() const { return m_size == 0; }
    void clear() { m_size = 0; m_more.clear(); }
    void resize(std::size_t n) {
        while (m_size > n) pop_back();
        while (m_size < n) push_back(T());
    }
private:
    std::size_t m_size;
    T m_embed[N];
    std::vector<T> m_more;
};

} // detail

/// @cond
}  // MSGPACK_API_VERSION_NAMESPACE(v2)
/// @endcond

}  // namespace msgpack

#endif // MSGPACK_V2_EMBED_STACK_HPP
