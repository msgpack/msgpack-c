FROM ubuntu:xenial

RUN apt-get update \
    && apt-get install -y software-properties-common \
    && apt-add-repository ppa:ubuntu-toolchain-r/test \
    && apt-get install -y curl \
    && curl -L http://llvm.org/apt/llvm-snapshot.gpg.key |apt-key add - \
    && add-apt-repository 'deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial main' \
    && apt-get -y install clang-5.0 lldb-5.0 wget unzip valgrind build-essential zlib1g-dev subversion

RUN mkdir -p /src /work

ENV SRC=/src
ENV WORK=/work
ENV CXX=clang++-5.0
ENV CC=clang-5.0
ENV ARCH=64
ENV BASE="/work"
ENV PATH="${BASE}/cmake/bin:$PATH"

RUN ln -s /usr/bin/clang++-5.0 /usr/bin/clang++ \
    && ln -s /usr/bin/clang-5.0 /usr/bin/clang

RUN cd $SRC \
    && wget -q --no-check-certificate https://cmake.org/files/v3.7/cmake-3.7.1-Linux-x86_64.sh -O \
    cmake-3.7.1-Linux-x86_64.sh \
    && chmod a+x cmake-3.7.1-Linux-x86_64.sh \
    && mkdir -p $WORK/cmake \
    && ./cmake-3.7.1-Linux-x86_64.sh --prefix=$WORK/cmake --skip-license \
    && rm $SRC/cmake-3.7.1-Linux-x86_64.sh

RUN cd $SRC \
    && svn co http://llvm.org/svn/llvm-project/llvm/trunk llvm \
    && (cd llvm/projects && svn co http://llvm.org/svn/llvm-project/libcxx/trunk libcxx) \
    && (cd llvm/projects && svn co http://llvm.org/svn/llvm-project/libcxxabi/trunk libcxxabi) \
    && mkdir libcxx_msan && cd libcxx_msan \
    && cmake ../llvm -DCMAKE_BUILD_TYPE=Release -DLLVM_USE_SANITIZER=Memory -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
    && make cxx -j$(nproc)

RUN cd $SRC \
    && wget -q https://github.com/google/googletest/archive/release-1.7.0.zip -O googletest-release-1.7.0.zip \
    && unzip -q googletest-release-1.7.0.zip \
    && cd $SRC/googletest-release-1.7.0 \
    && MSAN_CFLAGS="-fsanitize=memory -stdlib=libc++ -L$SRC/libcxx_msan/lib -lc++abi -I$SRC/libcxx_msan/include -I$SRC/libcxx_msan/include/c++/v1" \
    && cmake . -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_FLAGS="$MSAN_CFLAGS" -DCMAKE_CXX_FLAGS="$MSAN_CFLAGS" \
    && make -j$(nproc) \
    && $CXX src/gtest-all.cc -I. -Iinclude -c \
    && $CXX src/gtest_main.cc -I. -Iinclude -c \
    && ar -rv libgtest.a gtest-all.o \
    && ar -rv libgtest_main.a gtest_main.o \
    && mkdir -p $WORK/gtest \
    && mkdir -p $WORK/gtest/include \
    && cp -r include/gtest $WORK/gtest/include \
    && mv *.a $WORK/gtest \
    && rm $SRC/googletest-release-1.7.0.zip \
    && rm -rf $SRC/googletest-release-1.7.0

RUN cd $SRC \
    && wget http://sourceforge.net/projects/boost/files/boost/1.66.0/boost_1_66_0.zip \
    && unzip -q boost_1_66_0.zip

RUN cd $SRC/boost_1_66_0 \
    && mkdir $WORK/boost \

    && ./bootstrap.sh --with-toolset=clang \
    && ./b2 clean \
    && ./b2 toolset=clang \
       cxxflags="-stdlib=libc++ -L$SRC/libcxx_msan/lib -lc++abi -I$SRC/libcxx_msan/include -I$SRC/libcxx_msan/include/c++/v1" \
       linkflags="-stdlib=libc++ -L$SRC/libcxx_msan/lib -lc++abi -I$SRC/libcxx_msan/include -I$SRC/libcxx_msan/include/c++/v1" \
       -j$(nproc) \
       --prefix=$WORK/boost \
       address-model=${ARCH} \
       --with-chrono --with-context --with-system --with-timer install \
    && rm -rf $SRC/boost_1_66_0 \
    && rm $SRC/boost_1_66_0.zip

ENV LD_LIBRARY_PATH="$BASE/:$LD_LIBRARY_PATH"

#   CXX11="ON" BOOST="ON" SHARED="ON" CHAR_SIGN="unsigned" API_VERSION="3" X3_PARSE="ON" CMAKE_CXX_COMPILER="${CXX}" CMAKE_C_COMPILER="${CC}" CMAKE_LIBRARY_PATH="${BASE}/boost:${BASE}" GTEST_ROOT="${BASE}/gtest" BOOST_ROOT="${BASE}/boost" CFLAGS="-Werror -g" CXXFLAGS="-Werror -g" ci/build_cmake.sh