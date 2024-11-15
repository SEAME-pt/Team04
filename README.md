# SEAME-Course-24-25

# Table of Contents

- [1. Tooling](#1-tooling)
  - [1.1. Bazel](#11-bazel)
    - [Setup](#111-setup)
    - [Test Run](#112-test-run)
  - [1.2. Clang-Tidy](#12-clang-tidy)
    - [Clang-Tidy Analysis](#121-clang-tidy-analysis)
    - [Automatic Fixes Locally](#122-automatic-fixes-locally)
  - [1.3. Clang-Format](#13-clang-format)
    - [Clang-Format Analysis](#131-clang-format-analysis)
    - [Automatic Fixes Locally](#132-automatic-fixes-locally)
  - [1.4. Bazel Buildifier](#14-bazel-buildifier)
    - [How to Run](#141-how-to-run)

# 1. Tooling

## 1.1. Bazel

## 1.1.1 Setup

Follow step 1 and 2 of bazel documentation: <https://bazel.build/install/ubuntu>

## 1.1.2 Test Run

```bash
bazel run //examples/cpp:bin
```

## 1.2. Clang-Tidy

Clang-Tidy is a static analysis tool for C++ code. It helps identify potential issues, enforce coding standards, and suggest improvements to your code. This section describes how to integrate and use Clang-Tidy with your project for both analysis and automatic fixes.

### 1.2.1. Clang-Tidy Analysis

To run Clang-Tidy analysis on your codebase using Bazel, execute the following command:

```bash
bazel build //... \
  --aspects @bazel_clang_tidy//clang_tidy:clang_tidy.bzl%clang_tidy_aspect \
  --output_groups=report \
  --@bazel_clang_tidy//:clang_tidy_config=//:clang_tidy_config
```

The checks are define on .clang-tidy file.

### 1.2.2. Automatic Fixes Locally

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

## 1.3. Clang-Format

Clang-Format is a tool that automatically formats C++ source code according to a set of predefined or custom style rules. Below are instructions for running Clang-Format analysis and applying automatic fixes to your code locally.

### 1.3.1. Clang-Format Analysis

To check the formatting of your code without making any changes, use the following command. It will run `clang-format` in "dry-run" mode, which simulates formatting and reports any issues without modifying files.

```bash
find . -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.cc" -o -name "*.cxx" -o -name "*.hxx" | xargs clang-format --dry-run --Werror
```

- `find .` recursively searches for C++ source files in the current directory and its subdirectories.
- `-name` "*.cpp" -o -name "*.h" ... ensures all common C++ file extensions are included.
- `xargs clang-format --dry-run --Werror` applies clang-format to the found files, in "dry-run" mode (no changes are made), and treats any formatting violations as errors (--Werror).

### 1.3.2. Automatic Fixes Locally

To automatically fix the formatting of your code based on the predefined style, you can use the following command. It will rewrite the files in-place according to Clang-Format's rules.

```bash
find . -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.cc" -o -name "*.cxx" -o -name "*.hxx" | xargs clang-format -i
```

## 1.4. Bazel Buildifier

### 1.4.1. How to Run

`Buildifier` is a tool for formatting Bazel build files (e.g., `BUILD`, `WORKSPACE`). It automatically reformats these files according to a consistent style, improving readability and maintaining consistency across your project.

To run Buildifier execute the following command:

```bash
bazel run //bazel:buildifier
```
