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
    visibility = ["//:__pkg__"],
    deps = [
        ":qrc",
        "//instrument-cluster/qml/ClusterData:backend",
        "//instrument-cluster/qml/Components:components",
        "//instrument-cluster/qml/Constants:constants",
        "//instrument-cluster/src",
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
        "//instrument-cluster/qml/ClusterData:backend",
        "//instrument-cluster/qml/Components:components",
        "//instrument-cluster/qml/Constants:constants",
        "//instrument-cluster/src",
    ],
)
