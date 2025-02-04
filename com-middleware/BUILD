cc_binary(
    name = "bin",
    srcs = ["src/main.cpp"],
    features = ["warnings_critical_code_gcc"],
    linkopts = [
        "-lzmq",
    ],
    visibility = ["//:__pkg__"],
    deps = [
        "//com-middleware/src/can",
        "//com-middleware/src/exceptions",
        "//mq",
        "@cppzmq",
    ],
)
