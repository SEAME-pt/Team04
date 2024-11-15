# Coverage tooling

This project uses `llvm-cov` to generate coverage reports.

## 1. Key Features

1. **Local or Docker Setup:**
   You can generate coverage reports by either installing `llvm-cov` locally or using the project's Docker environment.

2. **Partial Coverage Reporting:**
   Currently, the reports are generated **without a baseline**, meaning only files touched during test execution are included.

   **TODO:** Implement baseline generation to include all project files in the coverage report, even those not covered by tests.

## 2. Setup

To set up the required tools, follow these steps:

### 2.1. Without Docker

1. Update packages:

```bash
sudo apt update
```

2. Install LLVM Install the default version of LLVM:

```bash
sudo apt install llvm
```

### 2.2. Within Docker

For Docker setup, refer to the [README.md](../../README.md)

## How to Run

To generate a coverage report, run the following command:

```bash
bazel run //tools:llvm_cov -- -t <test> -o <output_dir>
```

Replace <test> with the name of the test target and <output_dir> with the desired output directory for the report.

Open the generated `index.html` file in the output directory to review test coverage.
