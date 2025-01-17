cc_library(
    name = "carMove",
    srcs = ["carMove.cpp"],
    hdrs = ["carMove.hpp"],
    linkopts = select({
        "@platforms//cpu:x86_64": [
            "-L/usr/lib/x86_64-linux-gnu",
            "-lpigpio",
            "-lrt",
        ],
        "@platforms//cpu:aarch64": [
            "-L/usr/lib/aarch64-linux-gnu",
            "-lpigpio",
            "-lrt",
        ],
    }),
    visibility = ["//visibility:public"],
)
