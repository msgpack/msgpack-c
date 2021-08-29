#ifndef MSGPACK_CPP_VERSION_HPP
#define MSGPACK_CPP_VERSION_HPP

#if defined(_MSC_VER)
#  if _MSC_VER < 1900
#    define MSGPACK_USE_CPP03
#  endif
#elif (__cplusplus < 201103L)
#  define MSGPACK_USE_CPP03
#endif

#if defined(_MSVC_LANG)
#  define MSGPACK_CPP_VERSION _MSVC_LANG
#else
#  define MSGPACK_CPP_VERSION __cplusplus
#endif

#endif // MSGPACK_CPP_VERSION_HPP
