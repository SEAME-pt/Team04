load("@py_deps//:requirements.bzl", "requirement")

py_library(
    name = "mpc_lib",
    srcs = [
        "__init__.py",
        "mpc.py",
    ],
    imports = ["."],
    visibility = ["//applications/controller:__subpackages__"],
    deps = [
        requirement("numpy"),
        requirement("scipy"),
    ],
)

py_library(
    name = "runner_lib",
    srcs = [
        "__init__.py",
        "runner.py",
    ],
    imports = ["."],
    visibility = ["//applications/controller:__subpackages__"],
    deps = [
        requirement("numpy"),
        ":mpc_lib",
        "//applications/controller/misc:coordinate_transformer_lib",
        "//applications/controller/misc:utils_lib",
    ],
)
