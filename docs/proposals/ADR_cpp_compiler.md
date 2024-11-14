# ADR: Choosing a Compiler for C++

## Status

Proposed

## Context

We need to select a C++ compiler for a cross-platform project.

## Decision

We will use **Clang** as the primary C++ compiler for the project.

### Reasons for Choosing Clang

- **Cross-platform support**: Works well on Linux, macOS, and Windows.
- **Modern C++ support**: Supports the latest C++ standards (C++17, C++20, and C++23).
- **Performance**: Produces optimized code with small binary sizes.
- **Tool integration**: Works well with CMake, Ninja, and other build tools.
- **Error messages**: Provides clear and detailed error messages for easier debugging.
- **Tooling**: Includes useful tools like Clang-Tidy and Clang-Format for code quality.
- **Licensing**: Open-source and permissive license (University of Illinois/NCSA).

## Consequences

- **Toolchain**: We'll configure the build system to use Clang.

## Alternatives Considered

1. **GCC**:
   - Pros: Stable, mature, good optimizations.
   - Cons: Slower error reporting, less modern C++ standard support.

2. **MSVC**:
   - Pros: Best for Windows development.
   - Cons: Limited cross-platform support, slower adoption of modern C++ standards.

## Conclusion

Clang is the best choice for our project due to its cross-platform support, performance, modern C++ features, and strong tooling.
