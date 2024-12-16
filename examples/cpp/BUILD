cc_library(
    name = "lib",
    srcs = ["hello.cpp"],
    hdrs = ["hello.hpp"],
    features = select({
        "//bazel/platforms:aarch64_config": ["warnings_critical_code_gcc"],
        "//conditions:default": ["warnings_critical_code_clang"],
    }),
)

cc_binary(
    name = "bin",
    srcs = ["main.cpp"],
    features = select({
        "//bazel/platforms:aarch64_config": ["warnings_critical_code_gcc"],
        "//conditions:default": ["warnings_critical_code_clang"],
    }),
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
