"""
This module provides Bazel rules for working with Qt, including support for generating
Qt UI headers, processing Qt meta-object compiler (moc) files, and building binaries
with appropriate Qt library paths.
"""

load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

QT_INCLUDE_PATHS = select({
    "@platforms//cpu:x86_64": [
        "-I/opt/qt6/host/include",
        "-I/opt/qt6/host/include/QtGui",
        "-I/opt/qt6/host/include/QtWidgets",
        "-I/opt/qt6/host/include/QtCore",
    ],
    "@platforms//cpu:aarch64": [
        "-I/opt/qt6/pi/include",
        "-I/opt/qt6/pi/include/QtGui",
        "-I/opt/qt6/pi/include/QtWidgets",
        "-I/opt/qt6/pi/include/QtCore",
    ],
})

QT_LINK_PATHS = select({
    "@platforms//cpu:x86_64": [
        "-L/opt/qt6/host/lib",
        "-Wl,-rpath,/opt/qt6/host/lib",
        "-lQt6Widgets",
        "-lQt6Core",
        "-lQt6Gui",
    ],
    "@platforms//cpu:aarch64": [
        "-L/opt/qt6/pi/lib",
        "-Wl,-rpath,/opt/qt6/pi/lib",
        "-lQt6Widgets",
        "-lQt6Core",
        "-lQt6Gui",
    ],
})

qt_plugin_data = select({
    "@platforms//cpu:x86_64": ["/usr/lib/x86_64-linux-gnu", "/usr/lib/x86_64-linux-gnu/plugins", "/usr/lib/x86_64-linux-gnu/qml", "/opt/qt6/host/lib", "/opt/qt6/host/lib/plugins", "/opt/qt6/host/lib/qml"],
    "@platforms//cpu:aarch64": ["/usr/lib/x86_64-linux-gnu", "/usr/lib/aarch64-linux-gnu", "/usr/lib/aarch64-linux-gnu/plugins", "/usr/lib/aarch64-linux-gnu/qml", "/opt/qt6/pi/lib", "/opt/qt6/pi/lib/plugins", "/opt/qt6/pi/lib/qml"],
})

x86_64_env = {
    "QT_PLUGIN_PATH": "/opt/qt6/host/plugins",
    "QT_QPA_PLATFORM_PLUGIN_PATH": "/opt/qt6/host/plugins/platforms",
    "QML2_IMPORT_PATH": "/opt/qt6/host/qml",
}

aarch64_env = {
    "QT_PLUGIN_PATH": "/opt/qt6/pi/plugins",
    "QT_QPA_PLATFORM_PLUGIN_PATH": "/opt/qt6/pi/plugins/platforms",
    "QML2_IMPORT_PATH": "/opt/qt6/pi/qml",
}

def update_dict(source, env):
    result = {}
    result.update(source)
    result.update(env)
    return result

def qt_ui_library(name, ui, deps = None, **kwargs):
    """
    Creates a cc_library for a Qt UI file.

    This rule uses the Qt User Interface Compiler (uic) to generate a header file
    for the provided UI file and packages it as a Bazel `cc_library`.

    Args:
        name (str): The name of the library.
        ui (str): The path to the .ui file.
        deps (list): A list of dependencies for the generated library.
        **kwargs: Additional keyword arguments for `cc_library`.
    """
    native.genrule(
        name = "%s_uic" % name,
        srcs = [ui],
        outs = ["ui_%s.h" % ui.split(".")[0]],
        cmd = select({
            "@platforms//cpu:x86_64": "echo 'Running uic command: /opt/qt6/host/libexec/uic $(location %s) -o $@' && /opt/qt6/host/libexec/uic $(location %s) -o $@" % (ui, ui),
            "@platforms//cpu:aarch64": "echo 'Running uic command: /opt/qt6/host/libexec/uic $(location %s) -o $@' && /opt/qt6/host/libexec/uic $(location %s) -o $@" % (ui, ui),
        }),
    )

    hdr = ":%s_uic" % name
    cc_library(
        name = name,
        hdrs = [hdr],
        deps = deps,
        copts = QT_INCLUDE_PATHS,
        linkopts = QT_LINK_PATHS,
        **kwargs
    )

def qt_cc_library(name, srcs, hdrs, copts = [], linkopts = [], deps = [], **kwargs):
    """
    Creates a cc_library for a Qt component.

    This rule processes Qt header files with the Qt Meta-Object Compiler (moc)
    and includes them in the library along with the provided sources.

    Args:
        name (str): The name of the library.
        srcs (list): A list of source files.
        hdrs (list): A list of header files to process with moc.
        copts (list): Additional compiler options.
        linkopts (list): Additional linker options.
        deps (list): A list of dependencies for the library.
        **kwargs: Additional keyword arguments for `cc_library`.
    """
    _moc_srcs = []
    for hdr in hdrs:
        header_path = "%s/%s" % (native.package_name(), hdr) if len(native.package_name()) > 0 else hdr
        moc_name = "moc_%s" % hdr.rsplit(".", 1)[0]

        native.genrule(
            name = moc_name,
            srcs = [hdr],
            outs = [moc_name + ".cpp"],
            cmd = select({
                "@platforms//cpu:x86_64": "echo 'Running moc command: /opt/qt6/host/libexec/moc $(location %s) -o $@ -f \"%s\"' && /opt/qt6/host/libexec/moc $(location %s) -o $@ -f \"%s\"" % (hdr, header_path, hdr, header_path),
                "@platforms//cpu:aarch64": "echo 'Running moc command: /opt/qt6/host/libexec/moc $(location %s) -o $@ -f \"%s\"' && /opt/qt6/host/libexec/moc $(location %s) -o $@ -f \"%s\"" % (hdr, header_path, hdr, header_path),
            }),
        )

        _moc_srcs.append(":" + moc_name)

    cc_library(
        name = name,
        srcs = srcs + _moc_srcs,
        hdrs = hdrs,
        deps = deps,
        copts = copts + QT_INCLUDE_PATHS,
        linkopts = linkopts + QT_LINK_PATHS,
        **kwargs
    )

def qt_cc_binary(name, srcs, deps = None, copts = [], data = [], env = {}, **kwargs):
    """
    Creates a cc_binary for a Qt application.

    This rule generates a binary with the necessary environment variables and
    runtime paths for Qt libraries.

    Args:
        name (str): The name of the binary.
        srcs (list): A list of source files for the binary.
        deps (list): A list of dependencies for the binary.
        copts (list): Additional compiler options.
        data (list): Data files to include with the binary.
        env (dict): Additional environment variables for the binary.
        **kwargs: Additional keyword arguments for `cc_binary`.
    """
    x86_64_env_data = update_dict(x86_64_env, env)
    aarch64_env_data = update_dict(aarch64_env, env)
    env_file = []
    native.genrule(
        name = name + "_env",
        outs = ["qt_env.ini"],
        cmd = select({
            "@platforms//cpu:x86_64": "echo $$\"LD_LIBRARY_PATH: /usr/lib/x86_64-linux-gnu\" > $@ \
                    $$\"\r\nQT_QPA_PLATFORM_PLUGIN_PATH: /opt/qt6/host/plugins/platforms\" > $@ \
                    $$\"\r\nQML2_IMPORT_PATH: /opt/qt6/host/qml\" > $@ \
                    $$\"\r\nQT_PLUGIN_PATH: /opt/qt6/host/plugins\" > $@",
            "@platforms//cpu:aarch64": "echo $$\"LD_LIBRARY_PATH: /usr/lib/aarch64-linux-gnu\" > $@ \
                    $$\"\r\nQT_QPA_PLATFORM_PLUGIN_PATH: /opt/qt6/pi/plugins/platforms\" > $@ \
                    $$\"\r\nQML2_IMPORT_PATH: /opt/qt6/pi/qml\" > $@ \
                    $$\"\r\nQT_PLUGIN_PATH: /opt/qt6/lib/plugins\" > $@",
        }),
    )
    env_file.append("qt_env.ini")
    cc_binary(
        name = name,
        srcs = srcs,
        deps = deps,
        copts = copts + QT_INCLUDE_PATHS,
        data = env_file + data,
        env = select({
            "@platforms//cpu:x86_64": x86_64_env_data,
            "@platforms//cpu:aarch64": aarch64_env_data,
        }),
        **kwargs
    )
