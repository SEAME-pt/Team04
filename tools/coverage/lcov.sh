#!/bin/bash

set -euo pipefail
set -o noglob

usage() {
    echo "Usage:"
    echo "  -b TARGET_BASELINE      The target used to collect the baseline (-b \"target_1 target_2\")"
    echo "  -t TARGET_TESTS         The test target (-t \"target_1 target_2\")"
    echo "  -c GCNO_DIR             The path to the gcno files"
    echo "  -d GCDA_DIR             The path to the gcda files"
    echo "  -s SKIP_HTML_REPORT     Skip generation of HTML coverage report (default: false)."
    echo "  -o OUTPUT_DIR           Directory for output (default is temp directory)"
    echo "  -h                      Display this help message"
}

create_directory() {
    local dir="$1"
    if [ ! -d "$dir" ]; then
        echo "INFO: Creating directory: $dir"
        mkdir -p "$dir"
    fi
}

# Parse options
while getopts "b:t:c:d:o:sh" option; do
    case "${option}" in
    b) TARGET_BASELINE=${OPTARG} ;;
    t) TARGET_TESTS=${OPTARG} ;;
    c) GCNO_DIR=${OPTARG} ;;
    d) GCDA_DIR=${OPTARG} ;;
    s) SKIP_HTML_REPORT=true ;;
    o) OUTPUT_DIR=${OPTARG} ;;
    h | *)
        usage
        exit 1
        ;;
    esac
done

# Validate required arguments
if [[ -z "${TARGET_TESTS+x}" || -z "${GCNO_DIR+x}" || -z "${GCDA_DIR+x}" ]]; then
    echo "ERROR: Missing required arguments."
    usage
    exit 1
fi

# Set defaults
SKIP_HTML_REPORT="${SKIP_HTML_REPORT:-false}"
EXCLUDE_PATTERN="external|gtest|/usr"
TARGET_BASELINE="${TARGET_BASELINE:-"${TARGET_TESTS}"}"
OUTPUT_DIR="${OUTPUT_DIR:-"$(mktemp -d)/cov_report"}"

create_directory "$OUTPUT_DIR"

echo "INFO: Baseline target: ${TARGET_BASELINE}"
echo "INFO: Test target: ${TARGET_TESTS}"
echo "INFO: Output directory: ${OUTPUT_DIR}"

readonly WORKSPACE=$(cd "$(dirname "$(readlink -f "${0}")")" && bazel info workspace)

run_baseline() {
    echo -e "\nINFO: Building baseline target: ${TARGET_BASELINE}"
    bazel build \
        --collect_code_coverage \
        --experimental_fetch_all_coverage_outputs \
        --cxxopt="-O0" \
        -- ${TARGET_BASELINE}
}

run_coverage_tests() {
    echo -e "\nINFO: Running test target: ${TARGET_TESTS}"
    bazel coverage \
        --nocache_test_results \
        --experimental_fetch_all_coverage_outputs \
        --cxxopt="-O0" \
        --instrumentation_filter="[/:]" \
        -- ${TARGET_TESTS}
}

delete_coverage_files() {
    echo -e "\nINFO: Cleaning old .gcno and .gcda files"
    local directories=("${GCNO_DIR}" "${GCDA_DIR}")
    for dir in "${directories[@]}"; do
        find "$dir" -type f \( -name "*.gcno" -o -name "*.gcda" \) -exec chmod +w {} \; -exec rm -f {} \; || true
    done
}

generate_hashed_name() {
    local file_path="$1"
    local extension="${file_path##*.}"
    local path_no_extension="${file_path%.$extension}"
    local path_in_bin=$(echo "$path_no_extension" | awk -F"bin" "{print substr(\$0, length(\$1\"bin\") + 1)}")
    local hash=$(echo -n "$path_in_bin" | sha256sum | awk "{print \$1}")
    echo "$hash.$extension"
}

copy_coverage_files() {
    echo -e "\nINFO: Copy hashed .gcno and .gcda files"
    echo "Input directory: $1"
    echo "Output dir: $2"
    echo "Exclude pattern: $EXCLUDE_PATTERN"

    mkdir -p "$2"

    find "$1" -type f \( -name "*.gcda" -o -name "*.gcno" \) ! -path "*/external/*" ! -path "*gtest*" ! -path "*test*" | while IFS= read -r file; do
        new_name=$(generate_hashed_name "$file" "$1")
        cp "$file" "$2/$new_name"
        echo "Copied: $file -> $2/$new_name"
    done
}

main() {
    pushd "${WORKSPACE}" || return

    bazel clean
    delete_coverage_files

    run_baseline
    echo -e "\nINFO: Create baseline.info"
    lcov \
        --directory "${GCNO_DIR}" \
        --capture \
        -i \
        --ignore-errors source \
        --output-file "${OUTPUT_DIR}/baseline.info"

    echo -e "\nINFO: Filter baseline.info"
    lcov \
        --directory "${GCNO_DIR}" \
        --remove "${OUTPUT_DIR}/baseline.info" \
        '/usr/*' \
        '*external/*' \
        '*test*' \
        --ignore-errors unused \
        --ignore-errors empty \
        --ignore-errors source \
        --output-file "${OUTPUT_DIR}/baseline_filtered.info"

    run_coverage_tests
    echo -e "\nINFO: Create tests.info"
    lcov \
        --directory "${GCDA_DIR}" \
        --capture \
        --ignore-errors source \
        --ignore-errors mismatch \
        --output-file "${OUTPUT_DIR}/tests.info"

    echo -e "\nINFO: Filter tests.info"
    lcov \
        --directory "${GCDA_DIR}" \
        --remove "${OUTPUT_DIR}/tests.info" \
        '/usr/*' \
        'external/*' \
        '*test*' \
        --ignore-errors unused \
        --ignore-errors source \
        --output-file "${OUTPUT_DIR}/tests_filtered.info"

    echo -e "\nINFO: Merge baseline and tests coverage report"
    lcov \
        -a "${OUTPUT_DIR}/baseline_filtered.info" \
        -a "${OUTPUT_DIR}/tests_filtered.info" \
        -o "${OUTPUT_DIR}/coverage.info"

    if [ "${SKIP_HTML_REPORT}" = false ]; then
        echo -e "\nINFO: Generating HTML coverage report"
        genhtml \
            "${OUTPUT_DIR}/coverage.info" \
            --function-coverage \
            --branch-coverage \
            --prefix "/proc/self/cwd" \
            --ignore-errors source \
            --output-directory "${OUTPUT_DIR}"
    else
        echo -e "\nINFO: Generating coverage summary"
        lcov \
            --summary "${OUTPUT_DIR}/coverage.info"
    fi

    popd &>/dev/null || true
}

main
