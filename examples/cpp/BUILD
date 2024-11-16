cc_library(
    name = "lib",
    srcs = ["hello.cpp"],
    hdrs = ["hello.hpp"],
)

cc_binary(
    name = "bin",
    srcs = ["main.cpp"],
    copts = [
        "-Weverything",  # critical
    ],
    deps = [
        ":lib",
    ],
)

cc_test(
    name = "test",
    srcs = ["test.cpp"],
    deps = [
        ":lib",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
