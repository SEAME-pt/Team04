load("//bazel/rules:qt.bzl", "qt_cc_binary", "qt_resource")

qt_resource(
    name = "qrc",
    files = [
        "Main.qml",
        "ScreenUI.qml",
        "fonts/HelveticaNeue.ttf",
        "images/warning-light.png",
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
        ":qrc",
        "//instrument-cluster/mq",
        "//instrument-cluster/qml/ClusterData:backend",
        "//instrument-cluster/qml/Components:components",
        "//instrument-cluster/qml/Constants:constants",
        "//instrument-cluster/src",
    ],
)
