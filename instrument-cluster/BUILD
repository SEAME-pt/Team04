genrule(
    name = "process_ui",
    srcs = ["basicdisplay.ui"],
    outs = ["ui_basicdisplay.h"],
    cmd = "/usr/bin/uic $(SRCS) -o $(OUTS)",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "instrument_cluster_lib",  
    srcs = [
        "basicdisplay.cpp",
        "speed.cpp",
        "rpm.cpp",
        "zmqpublisher.cpp",
        "zmqsubscriber.cpp",
    ],
    hdrs = [
        "basicdisplay.h",
        "ui_basicdisplay.h",  
        "speed.h",
        "rpm.h",
        "zmqpublisher.h",
        "zmqsubscriber.h",
    ],
    deps = [":process_ui"],
    copts = [
        "-I.",  
        "-Ibazel-out/k8-fastbuild/bin",  
        "-Ibazel-out/k8-fastbuild/bin/instrument-cluster",
        "-I/usr/include/x86_64-linux-gnu/qt5",
        "-I/usr/include/x86_64-linux-gnu/qt5/QtGui",
        "-I/usr/include/x86_64-linux-gnu/qt5/QtWidgets",
        "-I/usr/include/x86_64-linux-gnu/qt5/QtCore",
    ]
)

cc_binary(
    name = "bin",
    srcs = ["main.cpp"],
    deps = [
        ":instrument_cluster_lib",
    ],
    linkopts = [
        "-lQt5Widgets",
        "-lQt5Core",
        "-lQt5Gui",
    ],
)
