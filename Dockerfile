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
    qtbase5-dev  \
    && apt-get clean

RUN wget -qO - https://apt.llvm.org/llvm-snapshot.gpg.key | tee /etc/apt/trusted.gpg.d/apt.llvm.org.asc \
    && echo "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-18 main" | tee -a /etc/apt/sources.list.d/llvm.list

RUN apt-get update && apt-get install -y \
    llvm-18 \
    llvm-18-dev \
    clang-18 \
    clang-tools-18 \
    clang-format-18 \
    clang-tidy-18 \
    lld-18 \
    lldb-18 \
    && rm -rf /var/lib/apt/lists/*

RUN pip install yamllint

RUN pip install gitlint

RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-lint/main/etc/install.sh | sh

RUN wget https://github.com/bazelbuild/bazelisk/releases/latest/download/bazelisk-linux-amd64 && \
    chmod 755 bazelisk-linux-amd64 && \
    mv bazelisk-linux-amd64 /usr/bin/bazelisk

ENV HOME=/root

RUN update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-18 100

RUN update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-18 100

RUN echo "alias bazel='bazelisk'" >> $HOME/.bashrc

RUN echo 'if [ -d "$HOME/bin" ] ; then\n    PATH="$HOME/bin:$PATH"\nfi' >> $HOME/.bashrc && \
    echo 'if [ -d "$HOME/usr/bin" ] ; then\n    PATH="$HOME/usr/bin:$PATH"\nfi' >> $HOME/.bashrc

RUN git config --global --add safe.directory /Team04
