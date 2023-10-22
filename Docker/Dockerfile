FROM debian:bookworm-slim

RUN apt update && apt install -y \
    clang-format \
    cmake \
    git \
    g++-12 \
    make \
    libgl-dev \
    libopengl0 \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev && \
    ln /usr/bin/g++-12 /usr/bin/g++
