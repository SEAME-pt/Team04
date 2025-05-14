load("@py_deps//:requirements.bzl", "requirement")

py_library(
    name = "utils_lib",
    srcs = [
        "__init__.py",
        "utils.py",
    ],
    imports = ["."],
    visibility = ["//applications/controller:__subpackages__"],
    deps = [
        requirement("numpy"),
        requirement("scipy"),
    ],
)
