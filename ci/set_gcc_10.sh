#!/bin/bash

version=10
priority=100

sudo update-alternatives --install /usr/bin/gcc         gcc         /usr/bin/gcc-${version}         ${priority}
sudo update-alternatives --install /usr/bin/gcc-ar      gcc-ar      /usr/bin/gcc-ar-${version}      ${priority}
sudo update-alternatives --install /usr/bin/gcc-nm      gcc-nm      /usr/bin/gcc-nm-${version}      ${priority}
sudo update-alternatives --install /usr/bin/gcc-ranlib  gcc-ranlib  /usr/bin/gcc-ranlib-${version}  ${priority}
sudo update-alternatives --install /usr/bin/gcov        gcov        /usr/bin/gcov-${version}        ${priority}
sudo update-alternatives --install /usr/bin/gcov-dump   gcov-dump   /usr/bin/gcov-dump-${version}   ${priority}
sudo update-alternatives --install /usr/bin/gcov-tool   gcov-tool   /usr/bin/gcov-tool-${version}   ${priority}
sudo update-alternatives --install /usr/bin/lto-dump    lto-dump    /usr/bin/lto-dump-${version}    ${priority}
sudo update-alternatives --install /usr/bin/g++         g++         /usr/bin/g++-${version}         ${priority}

sudo update-alternatives --install /usr/bin/cc   cc   /usr/bin/gcc-${version} ${priority}
sudo update-alternatives --install /usr/bin/c++  c++  /usr/bin/g++-${version} ${priority}
