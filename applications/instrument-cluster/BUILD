load("//bazel/rules:qt.bzl", "qt_cc_binary", "qt_resource")

qt_resource(
    name = "qrc",
    files = [
        "Main.qml",
        "ScreenUI.qml",
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
    visibility = ["//applications:__subpackages__"],
    deps = [
        ":qrc",
        "//applications/instrument-cluster/qml/ClusterData:backend",
        "//applications/instrument-cluster/qml/Components:components",
        "//applications/instrument-cluster/qml/Constants:constants",
        "//applications/instrument-cluster/src",
    ],
)

qt_cc_binary(
    name = "sim",
    srcs = [
        "main.cpp",
    ],
    copts = [
        "-DSIMULATION_MODE",
    ],
    linkopts = [
        "-lzmq",
    ],
    deps = [
        ":qrc",
        "//applications/instrument-cluster/qml/ClusterData:backend",
        "//applications/instrument-cluster/qml/Components:components",
        "//applications/instrument-cluster/qml/Constants:constants",
        "//applications/instrument-cluster/src",
    ],
)
