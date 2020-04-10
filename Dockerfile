FROM ubuntu:bionic-20200311

ARG CMAKE_VERSION=3.15.6

RUN apt-get update && apt-get install \
    && apt install -y curl wget \
    && wget -O cmake_install.sh https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/cmake-$CMAKE_VERSION-Linux-x86_64.sh \
    && chmod +x /cmake_install.sh \
    && /cmake_install.sh --skip-license

CMD [ "/bin/sh", "--version" ]