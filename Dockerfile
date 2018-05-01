FROM ubuntu:xenial

RUN apt-get update \
    && apt-get install -y software-properties-common wget unzip build-essential zlib1g-dev

# libc++-dev libc++abi-dev

# RUN add-apt-repository ppa:ubuntu-toolchain-r/test
#
# RUN wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add \
#     && apt-add-repository "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-3.5 main" \
#     && apt-get update
#

RUN apt-get install -y clang-3.5 \
    && ln -s /usr/bin/clang-3.5 /usr/bin/clang \
    && ln -s /usr/bin/clang++-3.5 /usr/bin/clang++

ENV PATH="${BASE}/usr/bin:$PATH" \
    LD_LIBRARY_PATH="${BASE}/usr/lib:$LD_LIBRARY_PATH" \
    CXX="clang++-3.5" \
    CC="clang-3.5" \
    ARCH="64"

RUN wget -q --no-check-certificate https://cmake.org/files/v3.7/cmake-3.7.1-Linux-x86_64.sh -O \
    cmake-3.7.1-Linux-x86_64.sh \
    && chmod a+x cmake-3.7.1-Linux-x86_64.sh \
    && ./cmake-3.7.1-Linux-x86_64.sh --prefix=${BASE}/usr --skip-license

RUN wget -q https://github.com/google/googletest/archive/release-1.7.0.zip -O googletest-release-1.7.0.zip \
    && unzip -q googletest-release-1.7.0.zip \
    && cd googletest-release-1.7.0 \
    && $CXX src/gtest-all.cc -I. -Iinclude -c \
    && $CXX src/gtest_main.cc -I. -Iinclude -c \
    && ar -rv libgtest.a gtest-all.o \
    && ar -rv libgtest_main.a gtest_main.o \
    && mkdir -p ${BASE}/usr/include \
    && cp -r include/gtest ${BASE}/usr/include \
    && mkdir -p ${BASE}/usr/lib \
    && mv *.a ${BASE}/usr/lib \
    && cd ..

RUN wget http://sourceforge.net/projects/boost/files/boost/1.66.0/boost_1_66_0.zip \
    && unzip -q boost_1_66_0.zip

# RUN cd boost_1_66_0 \
#     && ./bootstrap.sh --with-toolset=clang cxxflags="-std=c++11 -stdlib=libc++" linkflags="-stdlib=libc++" \
#     && ./b2 toolset=clang cxxflags="-stdlib=libc++" linkflags="-stdlib=libc++" -j3 --prefix=${BASE}/usr --with-chrono --with-context --with-system --with-timer address-model=${ARCH} install \
#     && cd .. # hihi

# ENV CXX11="ON" BOOST="ON" SHARED="ON" CHAR_SIGN="unsigned" API_VERSION="3" X3_PARSE="ON" SANITIZER="asan"
