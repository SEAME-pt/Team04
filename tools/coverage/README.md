# Coverage tooling

This project uses `lcov` to generate coverage reports.

## How to Run

To generate a coverage report, run the following command:

```bash
bazel run //tools/coverage:lcov -- -t <test_target> -b <baseline_target> -o <output_dir> -c $(bazel info bazel-bin) -d $(bazel info bazel-testlogs)
```

- `<test_target>` is the name of the test target.
- `<baseline_target>` corresponds to the target used to create the coverage baseline.
- `<output_dir>` with the desired output directory for the report.

Open the generated `index.html` file in the output directory to review test coverage.
