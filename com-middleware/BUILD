cc_binary(
    name = "bin",
    srcs = ["src/main.cpp"],
    features = ["warnings_critical_code_gcc"],
    linkopts = [
        "-lzmq",
    ],
    deps = [
        "//com-middleware/src/can",
        "//com-middleware/src/exceptions",
        "//com-middleware/src/mq",
        "@cppzmq",
    ],
)
