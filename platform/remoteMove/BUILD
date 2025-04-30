cc_library(
    name = "remoteMove",
    srcs = ["remoteMove.cpp"],
    hdrs = ["remoteMove.hpp"],
    linkopts = select({
        "@platforms//cpu:x86_64": [
            "-L/usr/lib/x86_64-linux-gnu",
            "-lSDL2",
        ],
        "@platforms//cpu:aarch64": [
            "-L/usr/lib/aarch64-linux-gnu",
            "-lSDL2",
        ],
    }),
    visibility = ["//visibility:public"],
    deps = [
        "//mq",
        "@cppzmq",
        "//platform/carMove",
    ],
)
