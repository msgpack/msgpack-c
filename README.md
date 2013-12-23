# Msgpack for C/C++

It's like JSON but small and fast.


## Overview

MessagePack is an efficient binary serialization format. It lets you exchange data among multiple languages like JSON. But it's faster and smaller. Small integers are encoded into a single byte, and typical short strings require only one extra byte in addition to the strings themselves.


## License

Msgpack is Copyright (C) 2008-2010 FURUHASHI Sadayuki and licensed under the Apache License, Version 2.0 (the "License"). For details see the `COPYING` file in this directory.


## Contributing

The source for msgpack-c is held at [msgpack-c](https://github.com/msgpack/msgpack-c) github.com site.

To report an issue, use the [msgpack-c issue tracker](https://github.com/msgpack/msgpack-c/issues) at github.com.


## Using Msgpack

### Building and Installing

#### Install from git repository

You will need gcc (4.1.0 or higher), autotools.

```
$ git clone https://github.com/msgpack/msgpack-c.git
$ cd msgpack-c
$ ./bootstrap
$ ./configure
$ make
$ sudo make install
```

#### Install from package

##### UNIX-like platform with ./configure

On typical UNIX-like platforms, download source package from [Releases](https://github.com/msgpack/msgpack-c/releases) and run `./configure && make && make install`. Example:

```
$ wget https://github.com/msgpack/msgpack-c/releases/download/cpp-0.5.8/msgpack-0.5.8.tar.gz
$ tar zxvf msgpack-0.5.8.tar.gz
$ cd msgpack-0.5.8
$ ./configure
$ make
$ sudo make install
```

##### FreeBSD with Ports Collection

On FreeBSD, you can use Ports Collection. Install [net/msgpack](http://www.freebsd.org/cgi/cvsweb.cgi/ports/devel/msgpack/) package.

##### Gentoo Linux with Portage

On Gentoo Linux, you can use emerge. Install [dev-libs/msgpack](http://gentoo-portage.com/dev-libs/msgpack) package.

##### Mac OS X with MacPorts

On Mac OS X, you can install MessagePack for C using MacPorts.

```
$ sudo port install msgpack
```

You might need to run `sudo port selfupdate` before installing to update the package repository.

You can also install via Homebrew.

```
$ sudo brew install msgpack
```


##### Windows

On Windows, download source package from [here](https://sourceforge.net/projects/msgpack/files/) and extract it. Open `msgpack_vc8.vcproj` or msgpack_vc2008 file and build it using batch build. It builds libraries in `lib/` folder and header files in `include/` folder.

You can build using command line as follows:

```
> vcbuild msgpack_vc2008.vcproj
> dir lib       % DLL files are here
> dir include   % header files are here
```


### Linking with an Application

Include `msgpack.hpp` (or `msgpack.h` for C) in your application and link with libmsgpack. Here is a typical gcc link command:

    gcc -lmsgpack myapp.c -o myapp


### Code Example
```CPP
#include <msgpack.hpp>
#include <vector>

int main(void) {
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
### Quickstart Guides

For more detailed examples see [QuickStart for C](QUICKSTART-C.md) and [QuickStart for C++](QUICKSTART-CPP.md).
