load("@rules_pkg//pkg:mappings.bzl", "pkg_attributes", "pkg_files")

filegroup(
    name = "bins_filegroup",
    srcs = [
        "//examples/baseCar",
        "//examples/cpp:bin",
        "//examples/gpio_sdl:bin",
        "//examples/qt/qml:bin",
        "//examples/qt/ui:bin",
    ],
)

pkg_files(
    name = "pkg_files",
    srcs = [
        ":bins_filegroup",
    ],
    attributes = pkg_attributes(
        group = "root",
        mode = "0755",
        owner = "root",
    ),
    strip_prefix = "/",
    visibility = ["//:__pkg__"],
)

test_suite(
    name = "unit_tests",
    tests = [
        "//examples/cpp:test",
    ],
)
