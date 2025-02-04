load("@rules_pkg//pkg:mappings.bzl", "pkg_filegroup")
load("@rules_pkg//pkg:tar.bzl", "pkg_tar")

pkg_filegroup(
    name = "release_pkg_filegroup",
    srcs = [
        "//com-middleware:pkg_files",
        "//examples:pkg_files",
    ],
)

pkg_tar(
    name = "release_bins",
    srcs = [
        ":release_pkg_filegroup",
    ],
    extension = "tar.gz",
)

test_suite(
    name = "unit_tests",
    tests = [
        "//examples:unit_tests",
        "//mq:test",
    ],
)
