import argparse
from tabulate import tabulate
from typing import Dict, Optional, List


def read_file(file_path: str) -> List[str]:
    with open(file_path, "r") as f:
        return f.readlines()


def parse_coverage_data(
    file_content: List[str], target_files: List[str]
) -> Dict[str, Optional[float]]:
    coverage_by_file = {}
    current_file = None
    total_lines = covered_lines = 0

    for line in file_content:
        if line.startswith("SF:"):
            current_file = line.strip().split("SF:")[1].replace("/proc/self/cwd/", "")
            if current_file in target_files:
                total_lines = covered_lines = 0
                coverage_by_file[current_file] = None

        if current_file in target_files:
            if line.startswith("LF:"):
                total_lines = int(line.split(":")[1])
            elif line.startswith("LH:"):
                covered_lines = int(line.split(":")[1])

            if total_lines > 0:
                coverage_by_file[current_file] = (covered_lines / total_lines) * 100
    return coverage_by_file


def compare_coverages(
    coverage_1: Dict[str, Optional[float]], coverage_2: Dict[str, Optional[float]]
) -> List[Dict[str, str]]:
    comparison_results = []
    all_files = coverage_1.keys() | coverage_2.keys()

    for file in all_files:
        coverage_1_value = coverage_1.get(file)
        coverage_2_value = coverage_2.get(file)

        if coverage_1_value is not None and coverage_2_value is not None:
            difference = coverage_2_value - coverage_1_value
        else:
            difference = "N/A"

        old_coverage = f"{coverage_1_value:.2f}%" if coverage_1_value is not None else "N/A"
        new_coverage = f"{coverage_2_value:.2f}%" if coverage_2_value is not None else "N/A"
        difference_str = f"{difference:.2f}%" if isinstance(difference, float) else "N/A"

        comparison_results.append(
            {
                "File": file,
                "Old_Coverage": old_coverage,
                "New_Coverage": new_coverage,
                "Difference": difference_str,
            }
        )
    return comparison_results


def format_as_table(data: List[Dict[str, str]]) -> str:
    headers = ["File", "Old Coverage", "New Coverage", "Difference"]
    rows = [
        [entry["File"], entry["Old_Coverage"], entry["New_Coverage"], entry["Difference"]]
        for entry in data
    ]
    rows.sort(key=lambda x: x[0])
    return tabulate(rows, headers=headers, tablefmt="github", stralign="center")


def save_table_to_file(table: str, output_file: str) -> None:
    with open(output_file, "w") as file:
        file.write(table)
    print(f"\nResults saved to {output_file}")


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Compare line coverage between two lcov coverage.info files."
    )
    parser.add_argument(
        "-c1",
        "--coverage1",
        required=True,
        help="Path to the first lcov coverage.info file",
    )
    parser.add_argument(
        "-c2",
        "--coverage2",
        required=True,
        help="Path to the second lcov coverage.info file",
    )
    parser.add_argument(
        "-t",
        "--target_files",
        required=True,
        nargs="+",
        help="List of files to compare coverage",
    )
    parser.add_argument(
        "-o",
        "--output_file",
        default="coverage_comparison.txt",
        help="Output file to save results",
    )

    args = parser.parse_args()

    coverage_data_1 = parse_coverage_data(
        read_file(args.coverage1), args.target_files
    )
    coverage_data_2 = parse_coverage_data(
        read_file(args.coverage2), args.target_files
    )

    coverage_comparison = compare_coverages(coverage_data_1, coverage_data_2)
    if coverage_comparison:
        table = format_as_table(coverage_comparison)
        print(table)
        save_table_to_file(table, args.output_file)
    else:
        print("Files not found on report")


if __name__ == "__main__":
    main()
