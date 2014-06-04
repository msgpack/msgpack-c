# Msgpack for C/C++

It's like JSON but small and fast.


## Overview

MessagePack is an efficient binary serialization format. It lets you exchange data among multiple languages like JSON. But it's faster and smaller. Small integers are encoded into a single byte, and typical short strings require only one extra byte in addition to the strings themselves.


## License

Msgpack is Copyright (C) 2008-2014 FURUHASHI Sadayuki and licensed under the Apache License, Version 2.0 (the "License"). For details see the `COPYING` file in this directory.


## Contributing

The source for msgpack-c is held at [msgpack-c](https://github.com/msgpack/msgpack-c) github.com site.

To report an issue, use the [msgpack-c issue tracker](https://github.com/msgpack/msgpack-c/issues) at github.com.


## Using Msgpack

### Header only library for C++
When you use msgpack on C++03 and C++11, you just add msgpack-c/include to your include path. You don't need to link any msgpack libraries.

e.g.)

```
g++ -I msgpack-c/include your_source_file.cpp
```

### Building and Installing

#### Install from git repository

##### Using autotools
You will need gcc (4.1.0 or higher), autotools.

For C++03 and C:

```
$ git clone https://github.com/redboltz/msgpack-c/tree/cxx_separate
$ cd msgpack-c
$ ./bootstrap
$ ./configure
$ make
$ sudo make install
```

For C++11:

```
$ git clone https://github.com/msgpack/msgpack-c.git
$ cd msgpack-c
$ ./bootstrap
$ ./configure CXXFLAGS="-std=c++11"
$ make
$ sudo make install
```

You need the compiler that fully supports C++11.

##### Using cmake
You will need gcc (4.1.0 or higher), cmake.

```
$ git clone https://github.com/msgpack/msgpack-c.git
$ cd msgpack-c
$ cmake .
$ make
```

If you want to setup C++11 version of msgpack, execute the following command:

```
$ git clone https://github.com/msgpack/msgpack-c.git
$ cd msgpack-c
$ cmake -DMSGPACK_CXX11=ON .
```

You need the compiler that fully supports C++11.

#### Install from package

Add msgpack-c/src to your include path.
```

For C++03:
```
$ wget https://github.com/msgpack/msgpack-c/releases/download/cpp-0.5.9/msgpack-0.5.9.tar.gz
$ tar zxvf msgpack-0.5.9.tar.gz
$ cd msgpack-0.5.9
$ ./configure

Add msgpack-c/src to your include path.
```

For C:
```
$ sudo brew install msgpack
```


##### Windows

Clone msgpack-c git repository.

```
$ git clone https://github.com/msgpack/msgpack-c.git
```

or using GUI git client. 

e.g.) tortoise git https://code.google.com/p/tortoisegit/

Launch cmake GUI client. http://www.cmake.org/cmake/resources/software.html

Set 'Where is the source code:' text box and 'Where to build the binaries:' text box.

Click 'Configure' button.

Choose your Visual Studio version.

Click 'Generate' button.

Open the created msgpack.sln on Visual Studio.

Build all.

### Linking with an Application for C

Include `msgpack.h` in your application and link with libmsgpack. Here is a typical gcc link command:

    gcc myapp.c -lmsgpack -o myapp

When you use the C++ version of the msgpack, you don't need to link any msgpack libraries.

### Code Example
```CPP
#include <msgpack.hpp>
#include <vector>
#include <string>
#include <iostream>

int main() {
    // This is target object.
    std::vector<std::string> target;
    target.push_back("Hello,");
    target.push_back("World!");

    // Serialize it.
    msgpack::sbuffer sbuf;  // simple buffer
    msgpack::pack(&sbuf, target);

    // Deserialize the serialized data.
    msgpack::unpacked msg;    // includes memory pool and deserialized object
    msgpack::unpack(&msg, sbuf.data(), sbuf.size());
    msgpack::object obj = msg.get();

    // Print the deserialized object to stdout.
    std::cout << obj << std::endl;    // ["Hello," "World!"]

    // Convert the deserialized object to staticaly typed object.
    std::vector<std::string> result;
    obj.convert(&result);

    // If the type is mismatched, it throws msgpack::type_error.
    obj.as<int>();  // type is mismatched, msgpack::type_error is thrown
}
```
