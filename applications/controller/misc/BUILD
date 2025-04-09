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

py_library(
    name = "coordinate_transformer_lib",
    srcs = [
        "__init__.py",
        "coordinate_transformer.py",
    ],
    imports = ["."],
    visibility = ["//applications/controller:__subpackages__"],
    deps = [
        requirement("numpy"),
        ":utils_lib",
    ],
)
