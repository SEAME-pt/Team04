#!/bin/bash

set -euo pipefail
set -o noglob

usage() {
    echo "  -t BAZEL_TESTS          The test target (e.g., //path/to/test-target)"
    echo "  -b BAZEL_OUTPUT         The bazel output base (e.g., $user/cov_cache)"
    echo "  -s SKIP_HTML_REPORT     Defines if html report generation should be skipped"
    echo "  -o OUTPUT_DIR           Directory for output (default is temp directory)"
    echo "  -h                      Display this help message"
}

while getopts "t:c:o:sh" option; do
    case "${option}" in
    t) BAZEL_TESTS=${OPTARG} ;;
    c) BAZEL_OUTPUT=${OPTARG} ;;
    s) SKIP_HTML_REPORT=true ;;
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
BAZEL_OUTPUT=${BAZEL_OUTPUT:-"${HOME}/.cache/coverage"}
SKIP_HTML_REPORT=${SKIP_HTML_REPORT:-false}
echo "INFO: Bazel cache: ${BAZEL_OUTPUT}"
echo "INFO: Tests target: ${BAZEL_TESTS}"
echo "INFO: Output directory: ${OUTPUT_DIR}"

readonly WORKSPACE=$(cd "$(dirname "$(readlink -f "${0}")")" && bazel info workspace)

function run_tests() {
    echo -e "\nInfo: run test targets
        Bazel output: ${BAZEL_OUTPUT}
        Test target: ${BAZEL_TESTS}\n"
    bazel --output_base="${BAZEL_OUTPUT}" test \
        -c dbg \
        --cxxopt=-O0 \
        --experimental_fetch_all_coverage_outputs \
        --nocache_test_results \
        --build_runfile_links \
        --strategy=TestRunner=standalone \
        --copt=-fcoverage-mapping \
        --copt=-fprofile-instr-generate \
        --linkopt=-fcoverage-mapping \
        --linkopt=-fprofile-instr-generate \
        --instrumentation_filter="[/:]" \
        -- ${BAZEL_TESTS}
}

function create_profdata() {
    PROFRAW_PATH=$(find "${BAZEL_OUTPUT}" -name "default.profraw")
    echo -e "\nINFO: merge raw profile data
        Bazel output: ${BAZEL_OUTPUT}
        Profile file: $PROFRAW_PATH
        Output file: ${BAZEL_OUTPUT}/coverage.profdata\n"
    llvm-profdata merge \
        -sparse "$PROFRAW_PATH" \
        -o ${BAZEL_OUTPUT}/coverage.profdata
}

function create_html_report() {
    echo -e "\nINFO: create html report
        Bazel output: ${BAZEL_OUTPUT}
        Profile files: ${BAZEL_OUTPUT}/coverage.profdata
        Output report html: ${OUTPUT_DIR}/index.html\n"
    SOURCE_FILES=$(find ${BAZEL_OUTPUT} \
        -type f \
        -name "*.o" |
        grep -vE "external/|test/.*")
    llvm-cov show "${SOURCE_FILES}" \
        --instr-profile="${BAZEL_OUTPUT}/coverage.profdata" \
        -format=html \
        -output-dir="${OUTPUT_DIR}"
}

function show_summary() {
    echo -e "\nINFO: show coverage summary
        Bazel output: ${BAZEL_OUTPUT}
        Profile files: ${BAZEL_OUTPUT}/coverage.profdata
        Output report html: ${OUTPUT_DIR}/index.html\n"
    SOURCE_FILES=$(find ${BAZEL_OUTPUT} \
        -type f \
        -name "*.o" |
        grep -vE "external/|test/.*")
    llvm-cov report "${SOURCE_FILES}" \
        --instr-profile="${BAZEL_OUTPUT}/coverage.profdata"
}

function main() {
    pushd "${WORKSPACE}" || return
    run_tests
    create_profdata
    show_summary
    if [ "${SKIP_HTML_REPORT}" = false ]; then
        create_html_report
    fi
    popd || return
}

main
