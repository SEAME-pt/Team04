load("//bazel/rules:qt.bzl", "qt_cc_binary", "qt_cc_library", "qt_ui_library")

qt_ui_library(
    name = "ui",
    ui = "mainwindow.ui",
)

qt_cc_library(
    name = "lib",
    srcs = ["mainwindow.cpp"],
    hdrs = ["mainwindow.h"],
    deps = [":ui"],
)

qt_cc_binary(
    name = "bin",
    srcs = ["main.cpp"],
    visibility = ["//examples:__pkg__"],
    deps = [":lib"],
)
