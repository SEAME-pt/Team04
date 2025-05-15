load("@rules_pkg//pkg:mappings.bzl", "pkg_attributes", "pkg_files")
load("@rules_pkg//pkg:tar.bzl", "pkg_tar")

filegroup(
    name = "release_bins_filegroup",
    srcs = [
        "//applications:bins_filegroup",
        "//com-middleware:bin",
        "//examples:bins_filegroup",
        "//platform:bins_filegroup",
    ],
)

pkg_files(
    name = "pkg_files",
    srcs = [
        ":release_bins_filegroup",
    ],
    attributes = pkg_attributes(
        group = "root",
        mode = "0755",
        owner = "root",
    ),
    strip_prefix = "/",
    visibility = ["//:__pkg__"],
)

pkg_tar(
    name = "release_bins",
    srcs = [
        ":pkg_files",
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
