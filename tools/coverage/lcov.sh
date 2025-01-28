#!/bin/bash

set -euo pipefail
set -o noglob

usage() {
    echo "  -b TARGET_BASELINE      The target used to collect the baseline"
    echo "  -t TARGET_TESTS         The test target"
    echo "  -c CACHE                The path to the bazel cache"
    echo "  -s SKIP_HTML_REPORT     Skip generation of HTML coverage report (default: false)."
    echo "  -o OUTPUT_DIR           Directory for output (default is temp directory)"
    echo "  -h                      Display this help message"
}

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

if [[ -z "${TARGET_TESTS+x}" ]]; then
    echo "ERROR: Missing required arguments."
    usage
    exit 1
fi

if [[ -z "${GCNO_DIR+x}" || -z "${GCDA_DIR+x}" ]]; then
    echo "ERROR: Missing required directories for .gcno or .gcda files."
    usage
    exit 1
fi

SKIP_HTML_REPORT="${SKIP_HTML_REPORT:-false}"
EXCLUDE_PATTERN="external/*|*gtest/*|/usr/*"
TARGET_BASELINE="${TARGET_BASELINE:-"${TARGET_TESTS}"}"
OUTPUT_DIR="${OUTPUT_DIR:-"$(mktemp -d)/cov_report"}"
TARGET_DIR="${OUTPUT_DIR}/gcno_gcda"
mkdir -p "${TARGET_DIR}"

echo "INFO: Baseline target: ${TARGET_BASELINE}"
echo "INFO: Tests target: ${TARGET_TESTS}"
echo "INFO: Output directory: ${OUTPUT_DIR}"

readonly WORKSPACE=$(cd "$(dirname "$(readlink -f "${0}")")" && bazel info workspace)

function run_baseline() {
    echo "\nINFO: Build baseline"
    echo "Baseline target: ${TARGET_BASELINE}"
    bazel build \
        --collect_code_coverage \
        --experimental_fetch_all_coverage_outputs \
        -- ${TARGET_BASELINE}
}

function run_coverage_tests() {
    echo "\nINFO: Run test"
    echo "Test target: ${TARGET_TESTS}"
    bazel coverage \
        --nocache_test_results \
        --experimental_fetch_all_coverage_outputs \
        --cxxopt="-O0" \
        -- ${TARGET_TESTS}
}

function generate_hashed_name() {
    local file_path="$1"
    local extension="${file_path##*.}"                                    # Get the file extension (e.g., .gcno or .gcda)
    local path_no_extension="${file_path%".$extension"}"                  # Remove extension
    path_in_root=$(echo "$path_no_extension" | sed 's/.*\(bin\/.*\)/\1/') # Get file name from bin
    local hash=$(echo -n "$path_in_root" | sha256sum | awk '{print $1}')  # Apply hash
    echo "$hash.$extension"                                               # Return the hashed name with the original extension
}

function delete_coverage_files() {
    echo "\nINFO: Clean old .gcno and .gcda files"
    echo "Input directory: $TARGET_DIR"
    find "$TARGET_DIR" -name "*.gcno" -exec rm -f {} \;
    find "$TARGET_DIR" -name "*.gcda" -exec rm -f {} \;
}

function collect_coverage_data() {
    echo "\nINFO: Copy hashed .gcno and .gcda files"
    echo "Input gcno directory: $GCNO_DIR"
    echo "Input gcda directory: $GCDA_DIR"
    echo "Output dir: $TARGET_DIR"
    echo "Exclude pattern: $EXCLUDE_PATTERN"

    find "$GCDA_DIR" -name "*.gcda" ! -path "$EXCLUDE_PATTERN" | while read -r file; do
        new_name=$(generate_hashed_name "$file" "$GCDA_DIR")
        cp "$file" "$TARGET_DIR/$new_name"
        echo "Copied gcda: $file -> $TARGET_DIR/$new_name"
    done

    find "$GCNO_DIR" -name "*.gcno" ! -path "$EXCLUDE_PATTERN" | while read -r file; do
        new_name=$(generate_hashed_name "$file" "$GCNO_DIR")
        cp "$file" "$TARGET_DIR/$new_name"
        echo "Copied gcno: $file -> $TARGET_DIR/$new_name"
    done
}

function calculate_coverage() {
    echo "\nINFO: Generate coverage data"
    echo "Output directory: $OUTPUT_DIR"
    lcov --directory "$TARGET_DIR" \
        --capture \
        --ignore-errors source \
        --output-file "${OUTPUT_DIR}/coverage.info"
    lcov --remove "${OUTPUT_DIR}/coverage.info" \
        '/usr/*' \
        'external/*' \
        '*gtest*' \
        --ignore-errors unused \
        --ignore-errors source \
        --output-file "${OUTPUT_DIR}/coverage_filtered.info"
    sed -i 's|/proc/self/cwd/||g' "${OUTPUT_DIR}/coverage_filtered.info" # Remove file prefix

    if [ "${SKIP_HTML_REPORT}" = false ]; then
        echo "\nINFO: Generate HTML coverage report"
        genhtml "${OUTPUT_DIR}/coverage_filtered.info" \
            --ignore-errors source \
            --output-directory "$OUTPUT_DIR"
    else
        echo "\nINFO: Generate coverage summary"
        lcov --summary "${OUTPUT_DIR}/coverage_filtered.info"
    fi
}

function main() {
    pushd "${WORKSPACE}" || return
    run_baseline
    run_coverage_tests
    delete_coverage_files
    collect_coverage_data
    calculate_coverage
    popd || return
}

main
