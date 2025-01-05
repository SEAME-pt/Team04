#!/bin/bash

set -e

usage() {
    echo "  -t BAZEL_TESTS         The test target (e.g., //path/to/test-target)"
    echo "  -o OUTPUT_DIR          Directory for output (default is temp directory)"
    echo "  -h                     Display this help message"
}

while getopts "t:o:h" option; do
    case "${option}" in
    t) BAZEL_TESTS=${OPTARG} ;;
    o) OUTPUT_DIR=${OPTARG} ;;
    h | *)
        usage
        exit 1
        ;;
    esac
done

if [[ -z "${BAZEL_TESTS}" ]]; then
    echo "ERROR: Missing required arguments."
    usage
    exit 1
fi

OUTPUT_DIR="${OUTPUT_DIR:-"$(mktemp -d)/cov_report"}"

echo "INFO: Tests target: ${BAZEL_TESTS}"
echo "INFO: Output directory: ${OUTPUT_DIR}"

readonly WORKSPACE=$(cd "$(dirname "$(readlink -f "${0}")")" && bazel info workspace)

function run_coverage_tests() {
    echo -e "\nINFO: Run test targets\n"
    bazel coverage \
        --nocache_test_results \
        --experimental_fetch_all_coverage_outputs \
        --experimental_split_coverage_postprocessing \
        --strategy=CoverageReport=local \
        --combined_report=lcov \
        -- ${BAZEL_TESTS}
}

function calculate_coverage() {
    echo -e "\nINFO: Generating HTML coverage report\n"
    genhtml --branch-coverage --output genhtml "$(bazel info output_path)/_coverage/_coverage_report.dat"
}

function main() {
    pushd "${WORKSPACE}" || return
    run_coverage_tests
    calculate_coverage
    popd || return
}

main
