def _qt6_toolchain(ctx):
    return cc_common.create_cc_toolchain_config_info(
        cc_toolchain_config=ctx.attr._qt6_lib_path,
    )

qt6_toolchain = rule(
    implementation=_qt6_toolchain,
    attrs={
        "_qt6_lib_path": attr.string(default="/usr/lib/qt6"),
    },
)
