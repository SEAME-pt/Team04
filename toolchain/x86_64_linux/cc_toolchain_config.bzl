"""
Toolchains
"""

load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load("@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl", "feature", "flag_group", "flag_set", "tool_path")

all_link_actions = [
    ACTION_NAMES.cpp_link_executable,
    ACTION_NAMES.cpp_link_dynamic_library,
    ACTION_NAMES.cpp_link_nodeps_dynamic_library,
]

all_compile_actions = [
    ACTION_NAMES.assemble,
    ACTION_NAMES.c_compile,
    ACTION_NAMES.clif_match,
    ACTION_NAMES.cpp_compile,
    ACTION_NAMES.cpp_header_parsing,
    ACTION_NAMES.cpp_module_codegen,
    ACTION_NAMES.cpp_module_compile,
    ACTION_NAMES.linkstamp_compile,
    ACTION_NAMES.lto_backend,
    ACTION_NAMES.preprocess_assemble,
]

def _impl(ctx):
    tool_paths = [
        tool_path(name = "gcc", path = "/usr/bin/x86_64-linux-gnu-gcc"),
        tool_path(name = "ld", path = "/usr/bin/x86_64-linux-gnu-ld"),
        tool_path(name = "ar", path = "/usr/bin/x86_64-linux-gnu-ar"),
        tool_path(name = "cpp", path = "/usr/bin/x86_64-linux-gnu-cpp18"),
        tool_path(name = "gcov", path = "/usr/bin/gcov"),
        tool_path(name = "llvm-cov", path = "/usr/bin/llvm-cov-18"),
        tool_path(name = "llvm-profdata", path = "/usr/bin/llvm-profdata-18"),
        tool_path(name = "nm", path = "/usr/bin/nm"),
        tool_path(name = "objdump", path = "/usr/bin/objdump"),
        tool_path(name = "strip", path = "/usr/bin/strip"),
        tool_path(name = "clang-tidy", path = "/usr/bin/clang-tidy-18"),
        tool_path(name = "clang-format", path = "/usr/bin/clang-format-18"),
    ]

    features = [
        feature(
            name = "default_compiler_flags",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = all_compile_actions,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-no-canonical-prefixes",
                                "-Wno-builtin-macro-redefined",
                                "-D__DATE__=\"redacted\"",
                                "-D__TIMESTAMP__=\"redacted\"",
                                "-D__TIME__=\"redacted\"",
                            ],
                        ),
                    ],
                ),
            ],
        ),
        feature(
            name = "default_linker_flags",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = all_link_actions,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-lstdc++",
                                "-lm",
                            ],
                        ),
                    ],
                ),
            ],
        ),
        feature(
            name = "stack_protector",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = all_link_actions + all_compile_actions,
                    flag_groups = [
                        flag_group(
                            flags = ["-fstack-protector-strong"],
                        ),
                    ],
                ),
            ],
        ),
        feature(
            name = "debug_symbols",
            enabled = False,
            flag_sets = [
                flag_set(
                    actions = all_link_actions,
                    flag_groups = [
                        flag_group(flags = ["-g"]),
                    ],
                ),
            ],
        ),
        feature(
            name = "pic",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = all_link_actions + all_compile_actions,
                    flag_groups = [
                        flag_group(flags = ["-fPIC"]),
                    ],
                ),
            ],
        ),
        # Sanitizer
        feature(
            name = "asan",
            enabled = False,
            flag_sets = [
                flag_set(
                    actions = all_link_actions + all_compile_actions,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-fsanitize=address",
                                "-fno-omit-frame-pointer",
                                "-O0",
                            ],
                        ),
                    ],
                ),
            ],
        ),
        feature(
            name = "ubsan",
            enabled = False,
            flag_sets = [
                flag_set(
                    actions = all_link_actions + all_compile_actions,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-fsanitize=undefined",
                                "-O0",
                            ],
                        ),
                    ],
                ),
            ],
        ),
        feature(
            name = "tsan",
            enabled = False,
            flag_sets = [
                flag_set(
                    actions = all_link_actions + all_compile_actions,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-fsanitize=thread",
                                "-O0",
                            ],
                        ),
                    ],
                ),
            ],
        ),
        feature(
            name = "warnings_critical_code_clang",
            enabled = False,
            flag_sets = [
                flag_set(
                    actions = all_link_actions + all_compile_actions,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-Weverything",
                                "-Werror",
                                "-Wno-c++98-compat",
                                "-Wno-c++98-compat-pedantic",
                            ],
                        ),
                    ],
                ),
            ],
        ),
        feature(
            name = "warnings_critical_code_gcc",
            enabled = False,
            flag_sets = [
                flag_set(
                    actions = all_link_actions + all_compile_actions,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-Wall",
                                "-Wextra",
                                "-Wpedantic",
                                "-Wshadow",
                                "-Wnon-virtual-dtor",
                                "-Wold-style-cast",
                                "-Wcast-align",
                                "-Wunused",
                                "-Woverloaded-virtual",
                                "-Wconversion",
                                "-Wsign-conversion",
                                "-Wmisleading-indentation",
                                "-Wnull-dereference",
                                "-Wformat=2",
                                "-Wimplicit-fallthrough",
                                "-Werror",
                            ],
                        ),
                    ],
                ),
            ],
        ),
        feature(
            name = "warnings_default",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = all_link_actions + all_compile_actions,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-Wall",
                                "-Wshadow",
                            ],
                        ),
                    ],
                ),
            ],
        ),
    ]

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        features = features,
        cxx_builtin_include_directories = [
            "/usr/include",
            "/usr/local/include",
            "/usr/lib/x86_64-linux-gnu",
            "/usr/lib/gcc/x86_64-linux-gnu",
        ],
        toolchain_identifier = "linux_x86_64-toolchain",
        host_system_name = "local",
        target_system_name = "unknown",
        target_cpu = "unknown",
        target_libc = "unknown",
        compiler = "gcc",
        abi_version = "unknown",
        abi_libc_version = "unknown",
        tool_paths = tool_paths,
    )

cc_toolchain_config = rule(
    implementation = _impl,
    attrs = {},
    provides = [CcToolchainConfigInfo],
)
