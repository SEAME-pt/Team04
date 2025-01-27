cc_binary(
    name = "bin",
    srcs = ["main.cpp"],
    features = ["warnings_critical_code_gcc"],
    linkopts = [
        "-lzmq",
    ],
    deps = [
        "//com-middleware/can",
        "//com-middleware/exceptions",
        "@cppzmq",
    ],
)
