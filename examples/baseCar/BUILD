cc_binary(
    name = "baseCar",
    srcs = ["main.cpp"],
    linkopts = [
        "-lzmq",  # Add ZeroMQ library
    ],
    visibility = ["//examples:__pkg__"],
    deps = [
        "//platform/joystick",
        "//platform/remoteMove",
    ],
)
