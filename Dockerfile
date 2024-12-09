FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    vim \
    git \
    curl \
    wget \
    python3 \
    python3-pip \
    cmake \
    mesa-common-dev \
    mesa-utils \
    libvulkan-dev \
    libxkbcommon-x11-0 \
    libxkbcommon-dev \
    software-properties-common \
    libgl1-mesa-dev \
    libxcb-xinerama0 \
    libxcb-xinput0 \
    libxcb-icccm4 \
    libxcb-image0 \
    libxcb-keysyms1 \
    libxcb-randr0 \
    libxcb-render-util0 \
    libxcb-shape0 \
    libxcb-sync1 \
    libxcb-xfixes0 \
    libxcb-xkb1 \
    libxcb-cursor0 \
    locales \
    clang \
    clang-format \
    clang-tidy \
    && apt-get update \
    && apt-get clean

RUN pip install yamllint

RUN wget https://github.com/bazelbuild/bazelisk/releases/download/v1.23.0/bazelisk-linux-amd64 && \
    chmod 755 bazelisk-linux-amd64 && \
    mv bazelisk-linux-amd64 /usr/bin/bazelisk
RUN echo "alias bazel='bazelisk'" >> ~/.bashrc
