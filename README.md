# SEAME-Course-24-25

# Table of Contents

- [1. Tooling](#1-tooling)
  - [1.1. Bazel](#11-bazel)
    - [Setup](#111-setup)
  - [1.2. Compiler](#12-compiler)
  - [1.3. Clang-Tidy](#13-clang-tidy)
    - [Run Analysis](#131-run-analysis)
    - [Apply Fixes](#132-apply-fixes)
  - [1.4. Clang-Format](#14-clang-format)
    - [Run Analysis](#141-run-analysis)
    - [Apply Fixes](#142-apply-fixes)
  - [1.5. Bazel Buildifier](#15-bazel-buildifier)
    - [How to Run](#151-how-to-run)

# 1. Tooling

## 1.1. Bazel

Bazel is a powerful, fast, and extensible build system.

### 1.1.1 Setup

To get started, follow steps 1 and 2 of the official [Bazel installation guide](https://bazel.build/install/ubuntu).

Once Bazel is installed, you can run a simple "Hello World" example by executing the following command:

```bash
bazel run //examples/cpp:bin
```

This will build and run the bin target from the examples/cpp directory.

## 1.2. Compiler

The build settings, such as the choice of compiler, the C++ standard version, and default warning options, are configured in the `.bazelrc` file. In this setup, the compiler is set to Clang, and the C++ standard version is defined as C++20.

## 1.3. Clang-Tidy

Clang-Tidy is a static analysis tool for C++ code. It helps identify potential issues, enforce coding standards, and suggest improvements to your code. This section describes how to integrate and use Clang-Tidy with your project for both analysis and automatic fixes.

### 1.3.1. Run Analysis

To run Clang-Tidy analysis on your codebase using Bazel, execute the following command:

```bash
bazel build //... \
  --aspects @bazel_clang_tidy//clang_tidy:clang_tidy.bzl%clang_tidy_aspect \
  --output_groups=report \
  --@bazel_clang_tidy//:clang_tidy_config=//:clang_tidy_config
```

The checks are define on .clang-tidy file.

### 1.3.2. Apply Fixes

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

## 1.4. Clang-Format

Clang-Format is a tool that automatically formats C++ source code according to a set of predefined or custom style rules. Below are instructions for running Clang-Format analysis and applying automatic fixes to your code locally.

### 1.4.1. Run Analysis

To check the formatting of your code without making any changes, use the following command. It will run `clang-format` in "dry-run" mode, which simulates formatting and reports any issues without modifying files.

```bash
find . -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.cc" -o -name "*.cxx" -o -name "*.hxx" | xargs clang-format --dry-run --Werror
```

- `find .` recursively searches for C++ source files in the current directory and its subdirectories.
- `-name` "*.cpp" -o -name "*.h" ... ensures all common C++ file extensions are included.
- `xargs clang-format --dry-run --Werror` applies clang-format to the found files, in "dry-run" mode (no changes are made), and treats any formatting violations as errors (--Werror).

### 1.4.2. Apply Fixes

To automatically fix the formatting of your code based on the predefined style, you can use the following command. It will rewrite the files in-place according to Clang-Format's rules.

```bash
find . -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.cc" -o -name "*.cxx" -o -name "*.hxx" | xargs clang-format -i
```

## 1.5. Bazel Buildifier

### 1.5.1. How to Run

`Buildifier` is a tool for formatting Bazel build files (e.g., `BUILD`, `WORKSPACE`). It automatically reformats these files according to a consistent style, improving readability and maintaining consistency across your project.

To run Buildifier execute the following command:

```bash
bazel run //bazel:buildifier
```
