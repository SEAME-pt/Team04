# Coverage Comparison Tool

This tool compares line coverage between two `lcov` coverage report files for specified target files.

## Usage with Bazel

To run the tool using Bazel, use the following command:

```bash
bazel run //tools/coverage/utils:lcov_compare -- -c1 <coverage_file_1> -c2 <coverage_file_2> -t <file1> <file2> ... -o <output_file>
```

### Arguments

- `-c1`: Path to the first `lcov` file (required)
- `-c2`: Path to the second `lcov` file (required)
- `-t`: List of target files to compare (required)
- `-o`: Output file to save the results (optional, default is `coverage_comparison.txt`)

### Output

```markdown
| File       | Old_Coverage | New_Coverage | Difference |
|------------|--------------|--------------|------------|
| file1.cpp  | 70.00%       | 75.00%       | +5.00%     |
| file2.cpp  | 80.00%       | 75.00%       | -5.00%     |
```
