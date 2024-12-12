cc_library(
    name = "lib",
    srcs = ["hello.cpp"],
    hdrs = ["hello.hpp"],
    features = [
        "warnings_critical_code",
    ],
)

cc_binary(
    name = "bin",
    srcs = ["main.cpp"],
    features = [
        "warnings_critical_code",
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
