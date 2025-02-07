cc_binary(
    name = "bin",
    srcs = ["main.cpp"],
    features = ["warnings_critical_code_gcc"],
    linkopts = [
        "-lzmq",
    ],
    visibility = ["//platform:__pkg__"],
    deps = [
        "//mq",
        "@cppzmq",
    ],
)
