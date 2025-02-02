cc_library(
    name = "lib",
    srcs = [
        "gpio_sdl.cpp",
    ],
    hdrs = [
        "gpio_sdl.h",
    ],
    features = ["warnings_critical_code_gcc"],
    linkopts = select({
        "@platforms//cpu:x86_64": [
            "-L/usr/lib/x86_64-linux-gnu",
            "-lpigpio",
            "-lrt",
            "-lSDL2",
        ],
        "@platforms//cpu:aarch64": [
            "-L/usr/lib/aarch64-linux-gnu",
            "-lpigpio",
            "-lrt",
            "-lSDL2",
        ],
    }),
)

cc_binary(
    name = "bin",
    srcs = ["main.cpp"],
    features = ["warnings_critical_code_gcc"],
    visibility = ["//examples:__pkg__"],
    deps = [
        ":lib",
    ],
)
