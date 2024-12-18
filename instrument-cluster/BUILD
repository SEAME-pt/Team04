load("@rules_qt//:qt.bzl", "qt_cc_binary", "qt_cc_library")

genrule(
    name = "process_ui",
    srcs = ["basicdisplay.ui"],
    outs = ["ui_basicdisplay.h"],
    cmd = "/usr/lib/qt6/libexec/uic $< -o $@",
    visibility = ["//visibility:public"],
)

# Define a biblioteca Qt
qt_cc_library(
    name = "lib",
    srcs = ["basicdisplay.cpp", "speed.cpp", "rpm.cpp", "zmqpublisher.cpp", "zmqsubscriber.cpp"],
    hdrs = [
        "basicdisplay.h",
        "ui_basicdisplay.h",
        "speed.h",
        "rpm.h",
        "zmqpublisher.h",
        "zmqsubscriber.h"
    ],
    deps = [
        "@rules_qt//:qt_core",
        "@rules_qt//:qt_widgets",
    ],
)


qt_cc_binary(
    name = "bin",
    srcs = ["main.cpp"],
    deps = [
        ":lib",
    ],
)
