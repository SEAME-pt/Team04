genrule(
    name = "generate_ui_basicdisplay_h",
    srcs = ["basicdisplay.ui"],
    outs = ["ui_basicdisplay.h"],
    cmd = "uic $< -o $@",
)

genrule(
    name = "generate_moc_basicdisplay",
    srcs = ["basicdisplay.h"],
    outs = ["moc_basicdisplay.cpp"],
    cmd = "moc $< -o $@",
)

genrule(
    name = "generate_moc_zmqsubscriber",
    srcs = ["zmqsubscriber.h"],
    outs = ["moc_zmqsubscriber.cpp"],
    cmd = "moc $< -o $@",
)

cc_library(
    name = "speed",
    srcs = ["speed.cpp"],
    hdrs = ["speed.h"],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "rpm",
    srcs = ["rpm.cpp"],
    hdrs = ["rpm.h"],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "config",
    hdrs = ["config.h"],
)

cc_library(
    name = "zmqsubscriber",
    srcs = [
        "zmqsubscriber.cpp",
        ":generate_moc_zmqsubscriber",
    ],
    hdrs = ["zmqsubscriber.h"],
    copts = [
        "-I/usr/include/x86_64-linux-gnu/qt5",
        "-I/usr/include/x86_64-linux-gnu/qt5/QtCore",
    ],
    linkopts = [
        "-lzmq",
        "-lQt5Core",
    ],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "zmqpublisher",
    srcs = ["zmqpublisher.cpp"],
    hdrs = ["zmqpublisher.h"],
    copts = [
        "-I/usr/include/x86_64-linux-gnu/qt5",
        "-I/usr/include/x86_64-linux-gnu/qt5/QtCore",
    ],
    linkopts = [
        "-lzmq",
        "-lQt5Core",
    ],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "basicdisplay",
    srcs = [
        "basicdisplay.cpp",
        ":generate_moc_basicdisplay",
    ],
    hdrs = [
        "basicdisplay.h",
        ":generate_ui_basicdisplay_h",
    ],
    deps = [
        ":speed",
        ":rpm",
        ":zmqsubscriber",
        ":zmqpublisher",
        ":config",
    ],
    copts = [
        "-I.",
        "-Ibazel-out/k8-fastbuild/bin",
        "-Ibazel-out/k8-fastbuild/bin/instrument-cluster",
        "-I/usr/include/x86_64-linux-gnu/qt5",
        "-I/usr/include/x86_64-linux-gnu/qt5/QtGui",
        "-I/usr/include/x86_64-linux-gnu/qt5/QtWidgets",
        "-I/usr/include/x86_64-linux-gnu/qt5/QtCore",
    ],
    linkopts = [
        "-lQt5Widgets",
        "-lQt5Core",
        "-lQt5Gui",
    ],
)

cc_binary(
    name = "bin",
    srcs = ["main.cpp"],
    deps = [
        ":basicdisplay",
        ":speed",
        ":rpm",
        ":zmqsubscriber",
        ":zmqpublisher",
        ":config",
    ],
    copts = [
        "-I.",
        "-Ibazel-out/k8-fastbuild/bin",
        "-Ibazel-out/k8-fastbuild/bin/instrument-cluster",
        "-I/usr/include/x86_64-linux-gnu/qt5",
        "-I/usr/include/x86_64-linux-gnu/qt5/QtGui",
        "-I/usr/include/x86_64-linux-gnu/qt5/QtWidgets",
        "-I/usr/include/x86_64-linux-gnu/qt5/QtCore",
    ],
    linkopts = [
        "-lQt5Widgets",
        "-lQt5Core",
        "-lQt5Gui",
    ],
)

exports_files([
    "speed.cpp",
    "rpm.cpp",
    "zmqsubscriber.cpp",
])
