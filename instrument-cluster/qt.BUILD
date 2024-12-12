genrule(
    name = "uic",
    srcs = ["//:basicdisplay.ui"],
    outs = ["basicdisplay.h"],
    cmd = "/usr/lib/qt6/libexec/uic $< -o $@"
)

cc_library(
    name = "qt6",
    hdrs = glob(["include/**/*.h"]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [],
)
