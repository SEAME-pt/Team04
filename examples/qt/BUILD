cc_binary(
    name = "bin",
    srcs = ["main.cpp"],
    copts = [
        "-I/usr/include/x86_64-linux-gnu/qt6",
        "-I/usr/include/x86_64-linux-gnu/qt6/QtGui",
        "-I/usr/include/x86_64-linux-gnu/qt6/QtWidgets",
        "-I/usr/include/x86_64-linux-gnu/qt6/QtCore",
    ],
    linkopts = [
        "-lQt6Widgets",
        "-lQt6Core",
        "-lQt6Gui",
    ],
)
