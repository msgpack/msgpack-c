//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2015 KONDO Takatoshi
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

#ifndef TEST_ALLOCATOR_HPP
#define TEST_ALLOCATOR_HPP

#include <memory>

namespace test {

template <typename T>
struct allocator {
    typedef typename std::allocator<T>::value_type value_type;
    typedef typename std::allocator<T>::pointer pointer;
    typedef typename std::allocator<T>::reference reference;
    typedef typename std::allocator<T>::const_pointer const_pointer;
    typedef typename std::allocator<T>::const_reference const_reference;
    typedef typename std::allocator<T>::size_type size_type;
    typedef typename std::allocator<T>::difference_type difference_type;
    template <class U> struct rebind { typedef allocator<U> other; };
#if defined(MSGPACK_USE_CPP03)
    allocator() throw() {}
    allocator (const allocator& alloc) throw()
        :alloc_(alloc.alloc_) {}
    template <class U>
    allocator (const allocator<U>& alloc) throw()
        :alloc_(alloc.alloc_) {}


    void construct ( pointer p, const_reference val ) {
        return alloc_.construct(p, val);
    }
    size_type max_size() const throw() { return alloc_.max_size(); }
#else  // defined(MSGPACK_USE_CPP03)
    allocator() noexcept {}
    allocator (const allocator& alloc) noexcept
        :alloc_(alloc.alloc_) {}
    template <class U>
    allocator (const allocator<U>& alloc) noexcept
        :alloc_(alloc.alloc_) {}
    template <class U, class... Args>
    void construct (U* p, Args&&... args) {
        return alloc_.construct(p, std::forward<Args>(args)...);
    }
    size_type max_size() const noexcept { return alloc_.max_size(); }
#endif // defined(MSGPACK_USE_CPP03)
    pointer allocate (size_type n) {
        return alloc_.allocate(n);
    }
    void deallocate (pointer p, size_type n) {
        return alloc_.deallocate(p, n);
    }
    void destroy (pointer p) {
        alloc_.destroy(p);
    }

    std::allocator<T> alloc_;
};

template <typename T>
inline bool operator==(allocator<T> const& lhs, allocator<T> const& rhs) {
    return lhs.alloc_ == rhs.alloc_;
}

template <typename T>
inline bool operator!=(allocator<T> const& lhs, allocator<T> const& rhs) {
    return lhs.alloc_ != rhs.alloc_;
}

} // namespace test

#endif // TEST_ALLOCATOR_HPP
