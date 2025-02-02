load("@rules_pkg//pkg:mappings.bzl", "pkg_attributes", "pkg_files")

cc_binary(
    name = "bin",
    srcs = ["src/main.cpp"],
    features = ["warnings_critical_code_gcc"],
    linkopts = [
        "-lzmq",
    ],
    deps = [
        "//com-middleware/src/can",
        "//com-middleware/src/exceptions",
        "//com-middleware/src/mq",
        "@cppzmq",
    ],
)

pkg_files(
    name = "pkg_files",
    srcs = [
        ":bin",
    ],
    attributes = pkg_attributes(
        group = "root",
        mode = "0755",
        owner = "root",
    ),
    strip_prefix = "/",
    visibility = ["//:__pkg__"],
)
