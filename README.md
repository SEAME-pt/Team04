# SEAME-Course-24-25

## Table of Contents

1. [Tooling](#1-tooling)
   - [Clang-Tidy](#11-clang-tidy)
     - [Clang-Tidy Analysis](#12-clang-tidy-analysis)
     - [Automatic Fixes Locally](#13-automatic-fixes-locally)

---

## 1. Tooling

### 1.1. Clang-Tidy

Clang-Tidy is a static analysis tool for C++ code. It helps identify potential issues, enforce coding standards, and suggest improvements to your code. This section describes how to integrate and use Clang-Tidy with your project for both analysis and automatic fixes.

---

### 1.2. Clang-Tidy Analysis

To run Clang-Tidy analysis on your codebase using Bazel, execute the following command:

```bash
bazel build //... \
  --aspects @bazel_clang_tidy//clang_tidy:clang_tidy.bzl%clang_tidy_aspect \
  --output_groups=report \
  --@bazel_clang_tidy//:clang_tidy_config=//:clang_tidy_config
```

The checks are define on .clang-tidy file.

### 1.3. Automatic Fixes Locally

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
