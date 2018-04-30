FROM debian:latest

RUN apt-get update && apt install -y build-essential wget unzip gcc-6 g++-6 unzip zlib1g-dev

RUN wget -q --no-check-certificate https://cmake.org/files/v3.7/cmake-3.7.1-Linux-x86_64.sh -O cmake-3.7.1-Linux-x86_64.sh && chmod a+x cmake-3.7.1-Linux-x86_64.sh && ./cmake-3.7.1-Linux-x86_64.sh --prefix=${BASE}/usr --skip-license

ENV PATH="${BASE}/usr/bin:$PATH" LD_LIBRARY_PATH="${BASE}/usr/lib:$LD_LIBRARY_PATH" CXX="g++-6" CC="gcc-6" ARCH="64"

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

RUN wget http://valgrind.org/downloads/valgrind-3.12.0.tar.bz2 && tar xjf valgrind-3.12.0.tar.bz2 \
    && cd valgrind-3.12.0 \
    && ./configure --prefix=${BASE}/usr > /dev/null \
    && make -j3 > /dev/null \
    && make install > /dev/null \
    && cd ..

RUN wget http://sourceforge.net/projects/boost/files/boost/1.66.0/boost_1_66_0.zip \
    && unzip -q boost_1_66_0.zip \
    && cd boost_1_66_0 \
    && ./bootstrap.sh \
    && ./b2 -j3 --prefix=${BASE}/usr --with-chrono --with-context --with-system --with-timer address-model=${ARCH} install > /dev/null \
    && cd ..

ENV CXX11="ON" BOOST="ON" SHARED="ON" CHAR_SIGN="unsigned" API_VERSION="3" X3_PARSE="ON"
