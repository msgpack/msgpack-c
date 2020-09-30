`msgpack` for C/C++
===================

It's like JSON but smaller and faster.

Overview
--------

[MessagePack](http://msgpack.org/) is an efficient binary serialization
format, which lets you exchange data among multiple languages like JSON,
except that it's faster and smaller. Small integers are encoded into a
single byte and short strings require only one extra byte in
addition to the strings themselves.

For better understanding refer to this image


![performance-optimization-techniques-of-messagepackruby-rubykaigi-2019-5-638](https://user-images.githubusercontent.com/72141924/94731549-97a90380-0382-11eb-9744-e0c6d688bfd2.jpg)

### C Library

See [c_master](https://github.com/msgpack/msgpack-c/tree/c_master)

### C++ Library

See [cpp_master](https://github.com/msgpack/msgpack-c/tree/cpp_master)

### Documentation

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
