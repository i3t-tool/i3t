FROM ubuntu:noble

RUN apt update && apt install -y \
    clang-format-16 \
    clang-tidy-17 \
    cmake \
    cppcheck \
    git \
    g++-12 \
    make \
    libgl-dev \
    libopengl0 \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    xvfb && \
    ln /usr/bin/g++-12 /usr/bin/g++
