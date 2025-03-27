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
