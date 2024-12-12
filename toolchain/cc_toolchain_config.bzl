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

def _impl(ctx):
    tool_paths = [
        tool_path(name = "gcc", path = "/usr/bin/clang-18"),
        tool_path(name = "ld", path = "/usr/bin/ld"),
        tool_path(name = "ar", path = "/usr/bin/ar"),
        tool_path(name = "cpp", path = "/usr/bin/clang-cpp18"),
        tool_path(name = "gcov", path = "/usr/bin/gcov"),
        tool_path(name = "llvm-cov", path = "/usr/bin/llvm-cov-18"),
        tool_path(name = "nm", path = "/usr/bin/nm"),
        tool_path(name = "objdump", path = "/usr/bin/objdump"),
        tool_path(name = "strip", path = "/usr/bin/strip"),
    ]

    features = [
        # Default linker flags
        feature(
            name = "default_linker_flags",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = all_link_actions,
                    flag_groups = [
                        flag_group(flags = ["-lstdc++", "-lm"]),
                    ],
                ),
            ],
        ),
        # Enable stack protector
        feature(
            name = "stack_protector",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = all_link_actions + [
                        ACTION_NAMES.c_compile,
                        ACTION_NAMES.cpp_compile,
                    ],
                    flag_groups = [
                        flag_group(
                            flags = ["-fstack-protector-strong"],  # Or use -fstack-protector/-fstack-protector-all
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
        # Enable position-independent code
        feature(
            name = "pic",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = [ACTION_NAMES.assemble, ACTION_NAMES.preprocess_assemble],
                    flag_groups = [
                        flag_group(flags = ["-fPIC"]),
                    ],
                ),
            ],
        ),
        # Enable Sanitizer
        feature(
            name = "asan",
            enabled = False,
            flag_sets = [
                flag_set(
                    actions = all_link_actions + [
                        ACTION_NAMES.c_compile,
                        ACTION_NAMES.cpp_compile,
                    ],
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-fsanitize=address",
                                "-fno-omit-frame-pointer",
                            ],
                        ),
                    ],
                ),
            ],
        ),
        # Enable UndefinedBehaviorSanitizer
        feature(
            name = "ubsan",
            enabled = False,
            flag_sets = [
                flag_set(
                    actions = all_link_actions + [
                        ACTION_NAMES.c_compile,
                        ACTION_NAMES.cpp_compile,
                    ],
                    flag_groups = [
                        flag_group(
                            flags = ["-fsanitize=undefined"],
                        ),
                    ],
                ),
            ],
        ),
        # Enable ThreadSanitizer
        feature(
            name = "tsan",
            enabled = False,
            flag_sets = [
                flag_set(
                    actions = all_link_actions + [
                        ACTION_NAMES.c_compile,
                        ACTION_NAMES.cpp_compile,
                    ],
                    flag_groups = [
                        flag_group(
                            flags = ["-fsanitize=thread"],
                        ),
                    ],
                ),
            ],
        ),
        # Warning options
        feature(
            name = "warnings_critical_code",
            enabled = False,
            flag_sets = [
                flag_set(
                    actions = all_link_actions + [
                        ACTION_NAMES.c_compile,
                        ACTION_NAMES.cpp_compile,
                    ],
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
            name = "warnings_medium_code",
            enabled = False,
            flag_sets = [
                flag_set(
                    actions = all_link_actions + [
                        ACTION_NAMES.c_compile,
                        ACTION_NAMES.cpp_compile,
                    ],
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
                                "-Wno-c++98-compat",
                                "-Wno-c++98-compat-pedantic",
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
                    actions = all_link_actions + [
                        ACTION_NAMES.c_compile,
                        ACTION_NAMES.cpp_compile,
                    ],
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-Wall",
                                "-Wshadow",
                                "-Wno-c++98-compat",
                                "-Wno-c++98-compat-pedantic",
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
            "/usr/lib/llvm-18/lib/clang/18/include",
            "/usr/lib/llvm-18/lib/clang/18/share",  # Add the directory containing `asan_ignorelist.txt`
            "/usr/include",
        ],
        toolchain_identifier = "local",
        host_system_name = "local",
        target_system_name = "local",
        target_cpu = "k8",
        target_libc = "unknown",
        compiler = "clang",
        abi_version = "unknown",
        abi_libc_version = "unknown",
        tool_paths = tool_paths,
    )

cc_toolchain_config = rule(
    implementation = _impl,
    attrs = {},
    provides = [CcToolchainConfigInfo],
)
