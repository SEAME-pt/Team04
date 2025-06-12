load("@py_deps//:requirements.bzl", "requirement")
load("@rules_python//python:defs.bzl", "py_binary")

py_binary(
    name = "main",
    srcs = ["src/main.py"],
    data = [
        ":images",
        ":models",
    ],
    visibility = ["//:__pkg__"],
    deps = [
        requirement("opencv-python"),
        requirement("onnxruntime-gpu"),
        requirement("scipy"),
        requirement("scikit-learn"),
        requirement("pyzmq"),
        "//multitask-model/src/models",
        requirement("numpy"),
        requirement("matplotlib"),
    ],
)

py_binary(
    name = "carlasim",
    srcs = ["src/carlasim.py"],
    data = [
        ":models",
    ],
    visibility = ["//:__pkg__"],
    deps = [
        requirement("opencv-python"),
        requirement("onnxruntime-gpu"),
        "//multitask-model/src/models",
        requirement("numpy"),
        requirement("pygame"),
        requirement("carla"),
    ],
)
