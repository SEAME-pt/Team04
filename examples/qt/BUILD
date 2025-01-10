cc_binary(
    name = "bin",
    srcs = ["main.cpp"],
    copts = [
        "-I/usr/include/aarch64-linux-gnu/qt5",
        "-I/usr/include/aarch64-linux-gnu/qt5/QtGui",
        "-I/usr/include/aarch64-linux-gnu/qt5/QtWidgets",
        "-I/usr/include/aarch64-linux-gnu/qt5/QtCore",
    ],
    linkopts = [
        "-lQt5Widgets",
        "-lQt5Core",
        "-lQt5Gui",
    ],
)
