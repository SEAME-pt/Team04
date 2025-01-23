FROM ubuntu:24.04


# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive \
    HOME=/root \
    LLVM_VERSION=18 \
    GCC_VERSION=14 \
    BAZELISK_URL=https://github.com/bazelbuild/bazelisk/releases/latest/download/bazelisk-linux-amd64 \
    BUILDIFIER_URL=https://github.com/bazelbuild/buildtools/releases/latest/download/buildifier-linux-amd64 \
    PIGPIO_URL=https://github.com/joan2937/pigpio \
    QT_MAJOR_VERSION="6.8" \
    QT_MINOR_VERSION="6.8.1"

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
    libxcb-util0-dev \
    libfontconfig1-dev \
    libfreetype-dev \
    libx11-dev \
    libx11-xcb-dev \
    libxcb-cursor-dev \
    libxcb-glx0-dev \
    libxcb-icccm4-dev \
    libxcb-image0-dev \
    libxcb-keysyms1-dev \
    libxcb-randr0-dev \
    libxcb-render-util0-dev \
    libxcb-shape0-dev \
    libxcb-shm0-dev \
    libxcb-sync-dev \
    libxcb-util-dev \
    libxcb-xfixes0-dev \
    libxcb-xinerama0-dev \
    libxcb-xkb-dev \
    libxcb1-dev \
    libxext-dev \
    libxfixes-dev \
    libxi-dev \
    libxkbcommon-dev \
    libxkbcommon-x11-dev \
    libxrender-dev \
    libxcb-xinput0 \
    libxcb-sync1 \
    libxcb-xkb1 \
    libxcb-cursor0 \
    locales \
    qt6-base-dev \
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
    libclang-${LLVM_VERSION}-dev \
    lld-${LLVM_VERSION} \
    lldb-${LLVM_VERSION} \
    && rm -rf /var/lib/apt/lists/*

# Set up GCC
RUN apt-get update && apt-get install -y \
    gcc-${GCC_VERSION} \
    g++-${GCC_VERSION} \
    gcc-aarch64-linux-gnu \
    g++-aarch64-linux-gnu \
    && rm -rf /var/lib/apt/lists/*

# Set up alternatives for LLVM and GCC tools
RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-${LLVM_VERSION} 100 && \
    update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-${LLVM_VERSION} 100 && \
    update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-${LLVM_VERSION} 100 && \
    update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-${LLVM_VERSION} 100 && \
    update-alternatives --install /usr/bin/llvm-cov llvm-cov /usr/bin/llvm-cov-${LLVM_VERSION} 100 && \
    update-alternatives --install /usr/bin/llvm-profdata llvm-profdata /usr/bin/llvm-profdata-${LLVM_VERSION} 100 && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-${GCC_VERSION} 100 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-${GCC_VERSION} 100

# Install Arduino lint, yamllint, and gitlint
RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-lint/main/etc/install.sh | sh && \
    pip install --break-system-packages yamllint gitlint

# Install Qt6 for x86_64 and aarch64
RUN apt-get update && apt-get install -y \
    ninja-build \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /build/sysroot /build/qt6/src /build/qt6/host /build/qt6/pi /opt/qt6/host /opt/qt6/pi

RUN pip install --break-system-packages gdown

RUN gdown 1EUZuWxTBTHegJAKcGsPDVEAZaag0pa3_ -O /build/rasp.tar.gz

RUN tar xvfz /build/rasp.tar.gz -C /build/sysroot

COPY tools/cross_compilation/qt/toolchain.cmake /build/qt6

RUN wget -q https://raw.githubusercontent.com/riscv/riscv-poky/master/scripts/sysroot-relativelinks.py && \
    chmod +x sysroot-relativelinks.py && \
    python3 sysroot-relativelinks.py /build/sysroot && \
    rm -f sysroot-relativelinks.py

RUN cd /build/qt6/src && \
    wget -q https://download.qt.io/official_releases/qt/${QT_MAJOR_VERSION}/${QT_MINOR_VERSION}/submodules/qtbase-everywhere-src-${QT_MINOR_VERSION}.tar.xz && \
    tar xf qtbase-everywhere-src-${QT_MINOR_VERSION}.tar.xz && \
    rm qtbase-everywhere-src-${QT_MINOR_VERSION}.tar.xz

RUN cd /build/qt6/host && \
    cmake /build/qt6/src/qtbase-everywhere-src-${QT_MINOR_VERSION} \
        -GNinja \
        -DCMAKE_BUILD_TYPE=Release \
        -DQT_BUILD_EXAMPLES=OFF \
        -DQT_BUILD_TESTS=OFF \
        -DCMAKE_INSTALL_PREFIX=/opt/qt6/host && \
    cmake --build . && \
    cmake --install .

RUN cd /build/qt6/pi && \
    cmake /build/qt6/src/qtbase-everywhere-src-${QT_MINOR_VERSION} \
        -GNinja \
        -DCMAKE_BUILD_TYPE=Release \
        -DINPUT_opengl=es2 \
        -DQT_BUILD_EXAMPLES=OFF \
        -DQT_BUILD_TESTS=OFF \
        -DQT_HOST_PATH=/opt/qt6/host \
        -DCMAKE_STAGING_PREFIX=/opt/qt6/pi \
        -DCMAKE_INSTALL_PREFIX=/usr/local/qt6 \
        -DCMAKE_TOOLCHAIN_FILE=/build/qt6/toolchain.cmake \
        -DQT_QMAKE_TARGET_MKSPEC=devices/linux-rasp-pi4-aarch64 \
        -DQT_FEATURE_xcb=ON \
        -DFEATURE_xcb_xlib=ON \
        -DQT_FEATURE_xlib=ON && \
    cmake --build . && \
    cmake --install .

# Install libzmq3-dev for x86
RUN echo 'deb http://download.opensuse.org/repositories/network:/messaging:/zeromq:/release-stable/xUbuntu_22.04/ /' | \
    tee /etc/apt/sources.list.d/network:messaging:zeromq:release-stable.list && \
    curl -fsSL https://download.opensuse.org/repositories/network:messaging:zeromq:release-stable/xUbuntu_22.04/Release.key | \
    gpg --dearmor | tee /etc/apt/trusted.gpg.d/network_messaging_zeromq_release-stable.gpg > /dev/null && \
    apt-get update && apt-get install -y libzmq3-dev && \
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
