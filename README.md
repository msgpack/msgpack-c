`msgpack` for C++
===================

Version 6.1.0 [![Build Status](https://github.com/msgpack/msgpack-c/workflows/CI/badge.svg?branch=cpp_master)](https://github.com/msgpack/msgpack-c/actions) [![Build status](https://ci.appveyor.com/api/projects/status/8kstcgt79qj123mw/branch/cpp_master?svg=true)](https://ci.appveyor.com/project/redboltz/msgpack-c/branch/cpp_master)
[![codecov](https://codecov.io/gh/msgpack/msgpack-c/branch/cpp_master/graph/badge.svg)](https://codecov.io/gh/msgpack/msgpack-c/branch/cpp_master)

It's like JSON but smaller and faster.

Overview
--------

[MessagePack](http://msgpack.org/) is an efficient binary serialization
format, which lets you exchange data among multiple languages like JSON,
except that it's faster and smaller. Small integers are encoded into a
single byte and short strings require only one extra byte in
addition to the strings themselves.

Example
-------

```c++
#include <msgpack.hpp>
#include <string>
#include <iostream>
#include <sstream>

int main()
{
    msgpack::type::tuple<int, bool, std::string> src(1, true, "example");

    // serialize the object into the buffer.
    // any classes that implements write(const char*,size_t) can be a buffer.
    std::stringstream buffer;
    msgpack::pack(buffer, src);

    // send the buffer ...
    buffer.seekg(0);

    // deserialize the buffer into msgpack::object instance.
    std::string str(buffer.str());

    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());

    // deserialized object is valid during the msgpack::object_handle instance is alive.
    msgpack::object deserialized = oh.get();

    // msgpack::object supports ostream.
    std::cout << deserialized << std::endl;

    // convert msgpack::object instance into the original type.
    // if the type is mismatched, it throws msgpack::type_error exception.
    msgpack::type::tuple<int, bool, std::string> dst;
    deserialized.convert(dst);

    // or create the new instance
    msgpack::type::tuple<int, bool, std::string> dst2 =
        deserialized.as<msgpack::type::tuple<int, bool, std::string> >();

    return 0;
}
```

See [`QUICKSTART-CPP.md`](./QUICKSTART-CPP.md) for more details.

Dependency
----------

msgpack-c requires [boost library](https://www.boost.org/).
C++ version of msgpack-c itself is a header-only library and depends only on
boost headers. Tests depend on boost unit test framework and are linked with
it, so if you want to build them, you need to have this dependency installed.

Experimental support for removing boost dependency

For cmake:

```
cmake -DMSGPACK_USE_BOOST=OFF ..
```

NOTE: `-DMSGPACK_BUILD_TESTS=ON` doesn't work with `-DMSGPACK_USE_BOOST=OFF`.

For C++ compiler

```
clang++ -DMSGPACK_NO_BOOST your_code.cpp
```


Usage
-----

- If you build your project with cmake, you can find msgpack-c with a
  canonical cmake-way:

  ```cmake
  # ...
  find_package(msgpack REQUIRED)
  # ...
  target_link_libraries(your_target_name <PRIVATE/PUBLIC/INTERFACE> msgpack-cxx)
  # ...
  ```

  This will search for `msgpack` cmake package in a system prefix and in
  prefixes from `CMAKE_PREFIX_PATH`. Note that msgpack-c depends on boost
  headers, and `msgpack` cmake package depends on `Boost` cmake package. The
  library is header-only and `target_link_libraries` command just adds path
  to msgpack-c headers to your compiler's include path.

  A usage example can be found at [test-install](test-install) directory.

- If you do not use cmake, you can just add path yo msgpack-c and boost
  headers to your include path:

  ```bash
  g++ -I msgpack-c/include -I path_to_boost your_source_file.cpp
  ```

Building and Installing
-----------------------

### Install from git repository

#### Using the Terminal (CLI)

You will need:

- `gcc >= 4.1.0`
- `cmake >= 3.1.0`

C++03:

```bash
git clone https://github.com/msgpack/msgpack-c.git
cd msgpack-c
git checkout cpp_master
cmake .
sudo cmake --build . --target install
```

If you want to build tests with different C++ version, you can use
`MSGPACK_CXX11`, `MSGPACK_CXX14`, `MSGPACK_CXX17`, `MSGPACK_CXX20` options.
Just replace the line

```bash
cmake .
```

with a line like that:

```bash
cmake -DMSGPACK_CXX20=ON .
```

Note that these flags do not affect installation. They just switch test cases.
All files are installed in every settings.

If you don't have superuser permissions or don't want to install the library
to a system-wide prefix, you can use `CMAKE_INSTALL_PREFIX` option like that:

```bash
cmake -DCMAKE_INSTALL_PREFIX=/your/custom/prefix .
```

Other useful options:

- `MSGPACK_BUILD_TESTS` (default `OFF`): build tests
- `MSGPACK_BUILD_EXAMPLES` (default `OFF`): build examples
- `MSGPACK_32BIT` (default `OFF`): 32bit compile
- `MSGPACK_USE_X3_PARSE` (default `OFF`): use Boost X3 parse
  (note that it requires C++14 or newer)
- `MSGPACK_CHAR_SIGN` (not set explicitly by default): char sign to use (signed or unsigned)
- `MSGPACK_USE_STATIC_BOOST` (default `OFF`): statically link with boost libraries

#### GUI on Windows

Clone msgpack-c git repository with the command:

```
git clone https://github.com/msgpack/msgpack-c.git
```

or using GUI git client (e.g. [tortoise git](https://code.google.com/p/tortoisegit/)).

1. Checkout to `cpp_master` branch

2. Launch [cmake GUI client](http://www.cmake.org/cmake/resources/software.html).

3. Set 'Where is the source code:' text box and
   'Where to build the binaries:' text box.

4. Click 'Configure' button.

5. Choose your Visual Studio version.

6. Click 'Generate' button.

7. Open the created msgpack.sln on Visual Studio.

8. Build all.

Documentation
-------------

You can get additional information including the tutorial on the
[wiki](https://github.com/msgpack/msgpack-c/wiki).

Contributing
------------

`msgpack-c` is developed on GitHub at [msgpack/msgpack-c](https://github.com/msgpack/msgpack-c).
To report an issue or send a pull request, use the
[issue tracker](https://github.com/msgpack/msgpack-c/issues).

Here's the list of [great contributors](https://github.com/msgpack/msgpack-c/graphs/contributors).

License
-------

`msgpack-c` is licensed under the Boost Software License, Version 1.0. See
the [`LICENSE_1_0.txt`](./LICENSE_1_0.txt) file for details.
