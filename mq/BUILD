cc_library(
    name = "mq",
    srcs = glob(["src/*.cpp"]),
    hdrs = glob(["src/*.hpp"]),
    features = ["warnings_critical_code_gcc"],
    visibility = ["//visibility:public"],
    deps = [
        "@cppzmq",
    ],
)

cc_test(
    name = "test",
    srcs = glob(["test/*.cpp"]),
    linkopts = [
        "-lzmq",
    ],
    deps = [
        ":mq",
        "@cppzmq",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
