load("//bazel/rules:qt.bzl", "qt_cc_library")

qt_cc_library(
    name = "src",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.hpp"]),
    visibility = ["//visibility:public"],
    deps = [
        "//applications/instrument-cluster/mq",
    ],
)
