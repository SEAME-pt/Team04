load("@rules_qt//:qt.bzl", "qt_cc_binary", "qt_cc_library")

qt_cc_library(
    name = "lib",
    srcs = ["display.cpp", "speed.cpp", display.ui],
    hdrs = ["display.h", "speed.h"],
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
