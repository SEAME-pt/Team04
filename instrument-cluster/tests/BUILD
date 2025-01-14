cc_test(
    name = "instrument_cluster_tests",
    srcs = [
        "main.cpp",          
        "tst_rpm.cpp",
        "tst_speed.cpp",
        "tst_zmqsubscriber.cpp",
    ],
    deps = [
        "//instrument-cluster:speed",
        "//instrument-cluster:rpm",
        "//instrument-cluster:zmqsubscriber",
    ],
    copts = [
        "-I.",
        "-I/usr/include/gtest/",
        "-I/usr/include/x86_64-linux-gnu/qt5",
        "-I/usr/include/x86_64-linux-gnu/qt5/QtGui",
        "-I/usr/include/x86_64-linux-gnu/qt5/QtWidgets",
        "-I/usr/include/x86_64-linux-gnu/qt5/QtCore",
    ],
    linkopts = [
        "-lQt5Widgets",
        "-lQt5Core",
        "-lQt5Gui",
        "-lgtest",
    ],
)
