cc_binary(
    name = "baseCar",
    srcs = ["main.cpp"],
    visibility = ["//examples:__pkg__"],
    deps = [
        "//platform/joystick",
        "//platform/remoteMove",
    ],
    linkopts = [
        "-lzmq",  # Add ZeroMQ library
    ],
)