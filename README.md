# SEAME-Course-24-25

## Table of Contents

- [1. Development Environment](#1-development-environment)
  - [1.1. Install Docker](#11-install-docker)
  - [1.2. Build the Docker Image](#12-build-the-docker-image)
  - [1.3. Run Docker](#13-run-docker)
  - [1.4. Common Issues](#14-common-issues)
- [2. Tooling](#2-tooling)
  - [2.1. Bazel](#21-bazel)
  - [2.2. Compiler](#22-compiler)
  - [2.3. Clang-Tidy](#23-clang-tidy)
    - [Run Analysis](#231-run-analysis)
    - [Apply Fixes](#232-apply-fixes)
  - [2.4. Clang-Format](#24-clang-format)
    - [Run Analysis](#241-run-analysis)
    - [Apply Fixes](#242-apply-fixes)
  - [2.5. Bazel Buildifier](#25-bazel-buildifier)
    - [How to Run](#251-how-to-run)

## 1. Development Environment

To set up the development environment with Docker, follow these steps:

### 1.1. Install Docker

```bash
sudo apt-get update
sudo apt-get install docker.io
```

### 1.2. Build the Docker Image

Once Docker is installed, build the Docker image with this command:

```bash
sudo docker build -t team04_docker .
```

Explanation:

- `--build-arg UID=$(id -u) --build-arg GID=$(id -g)`: These arguments pass the current user’s UID (User ID) and GID (Group ID) to the Docker build process. This ensures that files created by the container will have the correct ownership matching your local user and group.
- `-t team04_docker`: This flags the built image with the name team04_docker. You can choose a different name if needed.
- `.`: The dot (.) at the end specifies the current directory as the build context. Docker will use the Dockerfile in this directory to build the image.

Once the image is built, you can verify it by running docker images, which will display a list of available Docker images on your system.

### 1.3. Run Docker

To run the Docker container with the image and necessary volumes mounted, use the following command:

```bash
sudo docker run -t -i \
-v "$(pwd)":/Team04 \
--net=host \
--env="DISPLAY" \
--volume="$HOME/.Xauthority:/root/.Xauthority:rw" \
-it team04_docker bash
```

Explanation of flags used:

- `-v "$(pwd)":/Team04`: Mounts the current directory to /Team04 inside the container.
- `--net=host`: Uses the host network for the container, which is helpful for applications that require direct access to the host network (e.g., for GUI applications).
- `--env="DISPLAY"`: Passes the host’s display environment variable to the container for GUI applications.
- `--volume="$HOME/.Xauthority:/root/.Xauthority:rw"`: Allows access to the X server on the host, which is required for running GUI applications inside the container.
- `-it`: Runs the container interactively with a terminal.

### 1.4. Common Issues

If you encounter an error similar to the following when trying to run Qt applications:

```bash
Authorization required, but no authorization protocol specified
qt.qpa.xcb: could not connect to display :1
qt.qpa.plugin: Could not load the Qt platform plugin "xcb" in "external/qt_linux_x86_64/plugins/platforms" even though it was found.
This application failed to start because no Qt platform plugin could be initialized. Reinstalling the application may fix this problem.

Available platform plugins are: eglfs, wayland-egl, vnc, minimal, xcb, vkkhrdisplay, offscreen, wayland, linuxfb, minimalegl.
```

#### Solution: Allow Docker to Access the X Server

To resolve this, run the following command on the host machine to grant Docker containers access to your X server:

```bash
xhost +local:docker
```

This command allows local Docker containers to connect to the X server and use graphical interfaces.

## 2. Tooling

### 2.1. Bazel

Bazel is a powerful, fast, and extensible build system.

You can run a simple "Hello World" example by executing the following command:

```bash
bazel run //examples/cpp:bin
```

This will build and run the bin target from the examples/cpp directory.

### 2.2. Compiler

The build settings, such as the choice of compiler, the C++ standard version, and default warning options, are configured in the `.bazelrc` file. In this setup, the compiler is set to Clang, and the C++ standard version is defined as C++20.

### 2.3. Clang-Tidy

Clang-Tidy is a static analysis tool for C++ code. It helps identify potential issues, enforce coding standards, and suggest improvements to your code. This section describes how to integrate and use Clang-Tidy with your project for both analysis and automatic fixes.

#### 2.3.1. Run Analysis

To run Clang-Tidy analysis on your codebase using Bazel, execute the following command:

```bash
bazel build //... \
  --aspects @bazel_clang_tidy//clang_tidy:clang_tidy.bzl%clang_tidy_aspect \
  --output_groups=report \
  --@bazel_clang_tidy//:clang_tidy_config=//:clang_tidy_config
```

The checks are define on .clang-tidy file.

#### 2.3.2. Apply Fixes

To automatically apply fixes suggested by Clang-Tidy, follow these steps:

1. Generate compile_commands.json:

    This file contains the necessary compilation information for Clang-Tidy to analyze your project files. Run the following command to generate it:

    ```bash
    bazel run @hedron_compile_commands//:refresh_all
    ```

    This ensures that the compile_commands.json file is up-to-date and reflects the latest build settings.

2. Run Clang-Tidy with the -fix option:

    Now, you can apply Clang-Tidy fixes by running the following command:

    ```bash
    find . -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.cc" -o -name "*.cxx" -o -name "*.hxx" | xargs clang-tidy --header-filter=".*" -p ./compile_commands.json
    ```

    This command:
    Finds all relevant C++ source files in the project.
    Runs Clang-Tidy on each file using the compile_commands.json for accurate analysis.
    Applies any fixes Clang-Tidy suggests (e.g., code style corrections, best practice recommendations).

### 2.4. Clang-Format

Clang-Format is a tool that automatically formats C++ source code according to a set of predefined or custom style rules. Below are instructions for running Clang-Format analysis and applying automatic fixes to your code locally.

#### 2.4.1. Run Analysis

To check the formatting of your code without making any changes, use the following command. It will run `clang-format` in "dry-run" mode, which simulates formatting and reports any issues without modifying files.

```bash
find . -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.cc" -o -name "*.cxx" -o -name "*.hxx" | xargs clang-format --dry-run --Werror
```

- `find .` recursively searches for C++ source files in the current directory and its subdirectories.
- `-name` "*.cpp" -o -name "*.h" ... ensures all common C++ file extensions are included.
- `xargs clang-format --dry-run --Werror` applies clang-format to the found files, in "dry-run" mode (no changes are made), and treats any formatting violations as errors (--Werror).

#### 2.4.2. Apply Fixes

To automatically fix the formatting of your code based on the predefined style, you can use the following command. It will rewrite the files in-place according to Clang-Format's rules.

```bash
find . -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.cc" -o -name "*.cxx" -o -name "*.hxx" | xargs clang-format -i
```

### 2.5. Bazel Buildifier

#### 2.5.1. How to Run

`Buildifier` is a tool for formatting Bazel build files (e.g., `BUILD`, `WORKSPACE`). It automatically reformats these files according to a consistent style, improving readability and maintaining consistency across your project.

To run Buildifier execute the following command:

```bash
bazel run //bazel:buildifier
```
