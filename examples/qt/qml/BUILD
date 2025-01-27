load("//bazel/rules:qt.bzl", "qt_cc_binary", "qt_resource")

qt_resource(
    name = "qrc",
    files = [
        "main.qml",
    ],
)

qt_cc_binary(
    name = "bin",
    srcs = ["main.cpp"],
    deps = [
        ":qrc",
    ],
)
