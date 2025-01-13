# Base image
FROM ubuntu:22.04

# Variables
ENV HOME=/root
ENV PATH="$HOME/bin:$HOME/usr/bin:$PATH"
ARG QT_MAJOR_VERSION="6.8"
ARG QT_MINOR_VERSION="6.8.1"

# Install required dependencies
RUN apt-get update && apt-get install -y \
    vim \
    curl \
    lcov \
    file \
    python3-pip \
    wget \
    git \
    build-essential \
    make \
    cmake \
    rsync \
    sed \
    libclang-dev \
    ninja-build \
    gcc \
    bison \
    python3 \
    gperf \
    pkg-config \
    libfontconfig1-dev \
    libfreetype6-dev \
    libx11-dev \
    libx11-xcb-dev \
    libxext-dev \
    libxfixes-dev \
    libxi-dev \
    libxrender-dev \
    libxcb1-dev \
    libxcb-glx0-dev \
    libxcb-keysyms1-dev \
    libxcb-image0-dev \
    libxcb-shm0-dev \
    libxcb-icccm4-dev \
    libxcb-sync-dev \
    libxcb-xfixes0-dev \
    libxcb-shape0-dev \
    libxcb-randr0-dev \
    libxcb-render-util0-dev \
    libxcb-util-dev \
    libxcb-xinerama0-dev \
    libxcb-xkb-dev \
    libxkbcommon-dev \
    libxkbcommon-x11-dev \
    libatspi2.0-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    freeglut3-dev \
    libssl-dev \
    libgmp-dev \
    libmpfr-dev \
    libmpc-dev \
    flex \
    gawk \
    texinfo \
    libisl-dev \
    zlib1g-dev \
    libtool \
    autoconf \
    automake \
    libgdbm-dev \
    libdb-dev \
    libbz2-dev \
    libreadline-dev \
    libexpat1-dev \
    liblzma-dev \
    libffi-dev \
    libsqlite3-dev \
    libbsd-dev \
    perl \
    patch \
    m4 \
    libncurses5-dev \
    gettext \
    gcc-aarch64-linux-gnu \
    g++-aarch64-linux-gnu \
    binutils-aarch64-linux-gnu \
    libc6-arm64-cross \
    libc6-dev-arm64-cross \
    software-properties-common \
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
    glibc-source && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Set git safe directory
RUN git config --global --add safe.directory /Team04

# Add LLVM repo and install LLVM tools
RUN wget -qO - https://apt.llvm.org/llvm-snapshot.gpg.key | tee /etc/apt/trusted.gpg.d/apt.llvm.org.asc && \
    echo "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-18 main" | tee -a /etc/apt/sources.list.d/llvm.list && \
    apt-get update && apt-get install -y \
        llvm-18 \
        llvm-18-dev \
        clang-18 \
        clang-tools-18 \
        clang-format-18 \
        clang-tidy-18 \
        lld-18 \
        lldb-18 && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

# Set up clang tools alternatives
RUN update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-18 100 && \
    update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-18 100 && \
    update-alternatives --install /usr/bin/llvm-cov llvm-cov /usr/bin/llvm-cov-18 100 && \
    update-alternatives --install /usr/bin/llvm-profdata llvm-profdata /usr/bin/llvm-profdata-18 100

# Install Bazelisk
RUN wget https://github.com/bazelbuild/bazelisk/releases/latest/download/bazelisk-linux-amd64 && \
    chmod 755 bazelisk-linux-amd64 && \
    mv bazelisk-linux-amd64 /usr/bin/bazelisk

# Set up aliases
RUN echo "alias bazel='bazelisk'" >> $HOME/.bashrc

# Install linters
RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-lint/main/etc/install.sh | sh && \
    pip install yamllint gitlint

# Create required directories for Qt
RUN mkdir -p /build/sysroot /build/qt6/src /build/qt6/host /build/qt6/pi /opt/qt6/host /opt/qt6/pi

# Download the sysroot .tar file from Google Drive
RUN pip install gdown
RUN gdown 1EUZuWxTBTHegJAKcGsPDVEAZaag0pa3_ -O /build/rasp.tar.gz

# Copy files to build Qt for aarch64
RUN tar xvfz /build/rasp.tar.gz -C /build/sysroot
COPY tools/cross_compilation/qt/toolchain.cmake /build/qt6

# Fix symbolic links in the sysroot
RUN wget -q https://raw.githubusercontent.com/riscv/riscv-poky/master/scripts/sysroot-relativelinks.py && \
    chmod +x sysroot-relativelinks.py && \
    python3 sysroot-relativelinks.py /build/sysroot && \
    rm -f sysroot-relativelinks.py

# Download and extract Qt source on /build/qt/src
RUN cd /build/qt6/src && \
    wget -q https://download.qt.io/official_releases/qt/${QT_MAJOR_VERSION}/${QT_MINOR_VERSION}/submodules/qtbase-everywhere-src-${QT_MINOR_VERSION}.tar.xz && \
    tar xf qtbase-everywhere-src-${QT_MINOR_VERSION}.tar.xz && \
    rm qtbase-everywhere-src-${QT_MINOR_VERSION}.tar.xz

# Install Qt for host on /opt/qt6/host and store build files on /build/qt6/host
RUN cd /build/qt6/host && \
    cmake /build/qt6/src/qtbase-everywhere-src-${QT_MINOR_VERSION} \
        -GNinja \
        -DCMAKE_BUILD_TYPE=Release \
        -DQT_BUILD_EXAMPLES=OFF \
        -DQT_BUILD_TESTS=OFF \
        -DCMAKE_INSTALL_PREFIX=/opt/qt6/host && \
    cmake --build . --parallel 4 && \
    cmake --install .

# Install Qt for Raspberry Pi on /opt/qt6/pi and store build files on /build/qt6/pi
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
    cmake --build . --parallel 4 && \
    cmake --install .

# Move Qt host binaries, headers, and libs
RUN mkdir -p /usr/include/x86_64-linux-gnu/qt6 && \
    cp -r /opt/qt6/host/include/* /usr/include/x86_64-linux-gnu/qt6 && \
    cp -r /opt/qt6/host/lib/* /usr/lib/x86_64-linux-gnu

# Move Qt target binaries, headers, and libs
RUN mkdir -p /usr/include/aarch64-linux-gnu/qt6 && \
    mkdir -p /usr/lib/aarch64-linux-gnu && \
    cp -r /opt/qt6/pi/include/* /usr/include/aarch64-linux-gnu/qt6 && \
    cp -r /opt/qt6/pi/lib/* /usr/lib/aarch64-linux-gnu

# Move target binaries, headers, and libs
RUN cp -r /usr/aarch64-linux-gnu/lib/* /lib/aarch64-linux-gnu && \
    cp -r /usr/aarch64-linux-gnu/lib/ld-linux-aarch64.so.1 /lib

RUN echo 'if [ -d "$HOME/bin" ] ; then\n    PATH="$HOME/bin:$PATH"\nfi' >> $HOME/.bashrc && \
    echo 'if [ -d "$HOME/usr/bin" ] ; then\n    PATH="$HOME/usr/bin:$PATH"\nfi' >> $HOME/.bashrc
