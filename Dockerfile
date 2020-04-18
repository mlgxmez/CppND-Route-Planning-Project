FROM ubuntu:bionic-20200311

ARG CMAKE_VERSION=3.15.6

# Installation of CMake
RUN apt-get update && apt install -y curl wget build-essential libcairo2-dev libgraphicsmagick1-dev libpng-dev \
    && wget -O cmake_install.sh https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/cmake-$CMAKE_VERSION-Linux-x86_64.sh \
    && chmod +x /cmake_install.sh \
    && /cmake_install.sh --skip-license \
    && rm /cmake_install.sh

# Installation of IO2D
RUN apt install -y git && git clone --recurse-submodules https://github.com/cpp-io2d/P0267_RefImpl
WORKDIR P0267_RefImpl/Debug
RUN cmake --config Debug "-DCMAKE_BUILD_TYPE=Debug" .. \
    && cmake --build . \
    && make install
    
WORKDIR /home

ENV PATH="/usr/local/include/io2d:/usr/include/cairo:${PATH}"

RUN git clone --recurse-submodules https://github.com/mlgxmez/CppND-Route-Planning-Project

RUN mkdir build \
    && cd build \
    && cmake .. \
    && make

CMD [ "./test"]