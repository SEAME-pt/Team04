filegroup(
    name = "bins_filegroup",
    srcs = [
        "//examples/baseCar",
        "//examples/cpp:bin",
        "//examples/gpio_sdl:bin",
        "//examples/qt/qml:bin",
        "//examples/qt/ui:bin",
    ],
    visibility = ["//:__pkg__"],
)

test_suite(
    name = "unit_tests",
    tests = [
        "//examples/cpp:test",
    ],
)
