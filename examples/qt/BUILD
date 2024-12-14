cc_library(
    name = "lib",
    srcs = ["hello.cpp"],
    hdrs = [
        "hello.hpp",
    ],
    copts = [
        "-I/usr/include/x86_64-linux-gnu/qt5",
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
    copts = [
        "-I/usr/include/x86_64-linux-gnu/qt5",
        "-I/usr/include/x86_64-linux-gnu/qt5/QtWidgets",
        "-I/usr/include/x86_64-linux-gnu/qt5/QtCore",
    ],
    deps = [
        ":lib",
    ],
)
