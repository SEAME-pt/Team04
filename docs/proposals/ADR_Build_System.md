# ADR: Choosing Build System

## Status

Proposed (draft decision, needs review)

## Context

We are working on an autonomous driving system, which includes a variety of components such as:

- **Perception**: Sensor fusion, object detection, localization.
- **Planning**: Path planning, trajectory generation.
- **Control**: Vehicle control systems for steering, braking, and acceleration.
- **Simulation**: Simulation for testing, validation, and training.
- **Hardware Integration**: Interface with sensors, cameras, LIDAR, and vehicle control hardware.

Each of these components may involve different languages (e.g., C++, Python, Rust, etc.), and dependencies that evolve over time. As we scale up the project, the build system needs to support:

- Efficient management of interdependencies.
- Handling of both native and third-party libraries (including hardware SDKs).
- Fast incremental builds to avoid wasting time in large codebases.
- Integration with a CI/CD pipeline for testing and deployment.
- Ease of managing different environments (e.g., local dev, testing, production, simulation).

We are evaluating different build systems, including:

- **CMake**: A widely-used build system with good support for C++ and various languages. It is familiar to most developers and integrates well with common IDEs and toolchains.
- **Bazel**: A build system developed by Google that focuses on speed, scalability, and reproducibility. It supports multiple languages, can handle large repositories efficiently, and integrates well with modern CI/CD systems.
- **Make**: A more traditional build system, commonly used in C/C++ projects. It is highly configurable and fast for small-to-medium-sized projects but might struggle with larger, more complex systems.
- **Meson**: A build system known for speed and ease of use, with a focus on modern languages like Python and C++. It is becoming increasingly popular but does not yet have the maturity and widespread adoption of CMake or Bazel.

Our goal is to choose the build system that best meets the needs of our autonomous driving system while balancing developer experience, scalability, and integration with our CI/CD pipeline.

## Proposal

We are evaluating the use of **Bazel** as the build system for the autonomous driving system.

- **Scalability**: Bazel is designed to handle large codebases efficiently and supports fast, incremental builds even as the project grows. This is critical for an autonomous driving system, which involves many interdependent components (e.g., perception, control, planning).

- **Cross-Language Support**: Bazel has built-in support for multiple languages, including C++, Python, and Java. Since our project includes components in multiple languages (e.g., C++ for performance-sensitive control algorithms and Python for machine learning and simulation), Bazel allows us to manage these components in a unified way.

- **Dependency Management**: Bazel excels at managing dependencies and ensuring reproducible builds. This is especially important for integrating third-party libraries (e.g., sensor SDKs, machine learning frameworks) and ensuring that different subsystems are using compatible versions of shared libraries.

- **Parallel Builds**: Bazel’s advanced parallel build capabilities ensure that builds are fast, even for large repositories with complex dependencies. This will significantly improve development productivity by reducing build times.

- **CI/CD Integration**: Bazel integrates well with modern CI/CD pipelines and can be used to run tests, build, and deploy components in a fully automated manner. This is important for our autonomous driving project, where continuous testing and deployment are crucial to maintaining safety and performance.

- **Reproducibility**: Bazel is designed to provide deterministic and reproducible builds, which means we can ensure that the same build process will yield the same results across different environments (e.g., developer machines, CI servers, production systems).

## Consequences

### Positive Consequences:

- **Fast incremental builds**: Bazel will minimize build times, making it possible to test and integrate new features quickly, even as the project scales.
- **Unified build system**: Bazel allows us to manage both C++ and Python components in the same build system, reducing complexity and tooling overhead.
- **Integration with CI/CD**: Bazel’s integration with CI tools like Jenkins, GitLab, and GitHub Actions will streamline automated testing and deployment, allowing us to continuously validate our autonomous driving system.
- **Reproducibility and Reliability**: Bazel guarantees that builds will be consistent across different environments, which is critical for ensuring the reliability of the system.
- **Strong community and support**: Bazel has strong community backing and good documentation, making it easier to find resources and troubleshoot issues as they arise.

### Negative Consequences:

- **Learning Curve**: Bazel is more complex than traditional build systems like Make or CMake, and developers will need to learn its syntax, concepts (e.g., BUILD files, Starlark), and best practices.
- **Setup Time**: Initial configuration and migration to Bazel may take time, especially for integrating third-party libraries and setting up the build environment.
- **Tooling Ecosystem**: While Bazel has a robust ecosystem, some specialized tools (e.g., certain hardware SDKs) might require additional effort to integrate.

## Future Considerations:

- As the system grows, we may need to consider advanced features like remote caching to further optimize build times in the CI pipeline.
- Bazel’s support for multiple languages means that we can extend the project into new areas (e.g., machine learning) without having to switch build systems or introduce new tools.

## Alternatives Considered

### CMake

**Pros:**

- Familiar to many developers, especially those working in C++.
- Strong support for external libraries and integration with various toolchains.
- Widespread adoption in open-source and commercial projects.

**Cons:**

- Not as optimized for large-scale, multi-language projects.
- Builds can be slower than Bazel for large codebases, especially when handling complex dependencies.
- Limited support for advanced features like incremental builds and parallelization when compared to Bazel.

### Make

**Pros:**

- Simple and well-understood build system, especially for small-to-medium-sized C/C++ projects.
- Fast for small builds and simple projects.

**Cons:**

- Does not scale well for large projects with many interdependencies.
- Lacks cross-language support, meaning it would require significant configuration to support Python, Rust, or other languages used in the project.
- Manual configuration of dependencies and builds can become error-prone and hard to manage over time.

### Meson

**Pros:**

- Fast and easy to use for modern languages like Python and C++.
- Focuses on developer experience and speed.

**Cons:**

- Limited support for large, complex projects with multiple languages.
- Smaller community and ecosystem compared to Bazel, which may make troubleshooting or finding integrations more difficult.
- Does not have the same level of support for large-scale, parallel builds or advanced dependency management as Bazel.
