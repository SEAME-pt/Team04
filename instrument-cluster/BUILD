load("//bazel/rules:qt.bzl", "qt_cc_binary", "qt_cc_library", "qt_resource")

qt_resource(
    name = "qrc",
    files = [
        "Main.qml",
        "ScreenUI.qml",
    ],
)

qt_cc_library(
    name = "datamanager",
    srcs = ["DataManager.cpp"],
    hdrs = ["DataManager.hpp"],
    visibility = ["//visibility:public"],
    deps = [
        "//instrument-cluster/mq",
        "@cppzmq",
    ],
)

qt_cc_binary(
    name = "bin",
    srcs = [
        "main.cpp",
    ],
    linkopts = [
        "-lzmq",
    ],
    deps = [
        ":datamanager",
        ":qrc",
        "//instrument-cluster/mq",
    ],
)
