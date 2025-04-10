load("@py_deps//:requirements.bzl", "requirement")

py_binary(
    name = "generated_points",
    srcs = [
        "generated_points.py",
    ],
    main = "generated_points.py",
    deps = [
        "//applications/controller:mpc_lib",
        requirement("numpy"),
        requirement("matplotlib"),
        requirement("scipy"),
    ],
)

py_library(
    name = "data",
    srcs = [
        "data.py",
    ],
    imports = ["."],
    deps = [
        requirement("numpy"),
    ],
)

py_binary(
    name = "real_scenario",
    srcs = [
        "real_scenario.py",
    ],
    main = "real_scenario.py",
    deps = [
        ":data",
        "//applications/controller:runner_lib",
        requirement("numpy"),
        requirement("matplotlib"),
        requirement("scipy"),
    ],
)
