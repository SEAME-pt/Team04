FROM ubuntu:24.04

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive \
    LLVM_VERSION=18 \
    GCC_VERSION=12 \
    QT_VERSION=5.15.5 \
    QT_RELEASE_URL=https://github.com/parker-int64/qt-aarch64-binary/releases/download/5.15.5 \
    BAZELISK_URL=https://github.com/bazelbuild/bazelisk/releases/latest/download/bazelisk-linux-amd64 \
    BUILDIFIER_URL=https://github.com/bazelbuild/buildtools/releases/latest/download/buildifier-linux-amd64 \
    PIGPIO_URL=https://github.com/joan2937/pigpio \
    HOME=/root

# Update and install base dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    vim \
    git \
    curl \
    wget \
    python3 \
    python3-pip \
    cmake \
    lcov \
    file \
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
    qtbase5-dev \
    gcc-${GCC_VERSION}-aarch64-linux-gnu \
    g++-${GCC_VERSION}-aarch64-linux-gnu \
    libsdl2-dev \
    && rm -rf /var/lib/apt/lists/*

# Add LLVM repository and install LLVM/Clang tools
RUN wget -qO - https://apt.llvm.org/llvm-snapshot.gpg.key | tee /etc/apt/trusted.gpg.d/apt.llvm.org.asc && \
    echo "deb http://apt.llvm.org/noble/ llvm-toolchain-noble-${LLVM_VERSION} main" | tee /etc/apt/sources.list.d/llvm.list && \
    apt-get update && apt-get install -y \
    llvm-${LLVM_VERSION} \
    llvm-${LLVM_VERSION}-dev \
    clang-${LLVM_VERSION} \
    clangd-${LLVM_VERSION} \
    clang-tools-${LLVM_VERSION} \
    clang-format-${LLVM_VERSION} \
    clang-tidy-${LLVM_VERSION} \
    lld-${LLVM_VERSION} \
    lldb-${LLVM_VERSION} \
    && rm -rf /var/lib/apt/lists/*

# Set up GCC
RUN apt-get update && apt-get install -y \
    gcc-${GCC_VERSION} \
    g++-${GCC_VERSION} \
    && rm -rf /var/lib/apt/lists/*

# Set up alternatives for LLVM and GCC tools
RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-${LLVM_VERSION} 100 && \
    update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-${LLVM_VERSION} 100 && \
    update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-${LLVM_VERSION} 100 && \
    update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-${LLVM_VERSION} 100 && \
    update-alternatives --install /usr/bin/llvm-cov llvm-cov /usr/bin/llvm-cov-${LLVM_VERSION} 100 && \
    update-alternatives --install /usr/bin/llvm-profdata llvm-profdata /usr/bin/llvm-profdata-${LLVM_VERSION} 100 && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-${GCC_VERSION} 100 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-${GCC_VERSION} 100 && \
    update-alternatives --install /usr/bin/aarch64-linux-gnu-gcc aarch64-linux-gnu-gcc /usr/bin/aarch64-linux-gnu-gcc-{GCC_VERSION} 100 && \
    update-alternatives --install /usr/bin/x86_64-linux-gnu-gcc x86_64-linux-gnu-gcc /usr/bin/x86_64-linux-gnu-gcc-{GCC_VERSION} 100

# Install Arduino lint, yamllint, and gitlint
RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-lint/main/etc/install.sh | sh && \
    pip install --break-system-packages yamllint gitlint

# Download and extract precompiled Qt aarch64 libs
RUN wget ${QT_RELEASE_URL}/qt-${QT_VERSION}-aarch64-cross-compile-gcc-5.tar.gz -O /tmp/qt-aarch64-binary.tar.gz && \
    mkdir -p /opt/qt && \
    tar -xzf /tmp/qt-aarch64-binary.tar.gz -C /opt/qt && \
    rm /tmp/qt-aarch64-binary.tar.gz && \
    mv /opt/qt/qt-${QT_VERSION}-aarch64/lib/* /usr/lib/aarch64-linux-gnu && \
    cp /usr/aarch64-linux-gnu/lib/* /lib/aarch64-linux-gnu && \
    cp /usr/aarch64-linux-gnu/lib/ld-linux-aarch64.so.1 /lib

# Replace ubuntu.sources config file
COPY config/ubuntu.sources /tmp/ubuntu.sources
RUN cp /etc/apt/sources.list.d/ubuntu.sources /etc/apt/sources.list.d/ubuntu.sources.bak && \ 
    mv /tmp/ubuntu.sources /etc/apt/sources.list.d/ubuntu.sources

# Adding foreign architecture to 
RUN dpkg --add-architecture arm64

# Install libzmq3-dev
RUN echo 'deb http://download.opensuse.org/repositories/network:/messaging:/zeromq:/release-stable/xUbuntu_22.04/ /' | \
    tee /etc/apt/sources.list.d/network:messaging:zeromq:release-stable.list && \
    curl -fsSL https://download.opensuse.org/repositories/network:messaging:zeromq:release-stable/xUbuntu_22.04/Release.key | \
    gpg --dearmor | tee /etc/apt/trusted.gpg.d/network_messaging_zeromq_release-stable.gpg > /dev/null && \
    apt-get update && apt-get install -y libzmq3-dev && apt-get install -y libzmq3-dev:arm64 && \
    rm -rf /var/lib/apt/lists/*

# Install pigpio for x86
RUN git clone ${PIGPIO_URL}.git && \
    cd pigpio && \
    git checkout v79 && \
    make && \
    make install && \
    rm -rf pigpio

# Install Bazelisk
RUN wget ${BAZELISK_URL} && \
    chmod 755 bazelisk-linux-amd64 && \
    mv bazelisk-linux-amd64 /usr/bin/bazelisk && \
    ln -s /usr/bin/bazelisk /usr/bin/bazel

# Install Buildifier
RUN wget ${BUILDIFIER_URL} && \
    chmod 755 buildifier-linux-amd64 && \
    mv buildifier-linux-amd64 /usr/bin/buildifier

# Add bin paths to PATH
RUN echo 'if [ -d "$HOME/bin" ]; then PATH="$HOME/bin:$PATH"; fi' >> $HOME/.bashrc && \
    echo 'if [ -d "$HOME/usr/bin" ]; then PATH="$HOME/usr/bin:$PATH"; fi' >> $HOME/.bashrc

# Configure Git
RUN git config --global --add safe.directory /Team04
