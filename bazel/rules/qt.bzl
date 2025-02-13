"""
This module provides Bazel rules for working with Qt, including support for generating
Qt UI headers, processing Qt meta-object compiler (moc) files, and building binaries
with appropriate Qt library paths.
"""

load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

QT_INCLUDE_PATHS = select({
    "@platforms//cpu:x86_64": [
        "-isystem/usr/include/x86_64-linux-gnu/qt6",
        "-isystem/usr/include/x86_64-linux-gnu/qt6/QtGui",
        "-isystem/usr/include/x86_64-linux-gnu/qt6/QtWidgets",
        "-isystem/usr/include/x86_64-linux-gnu/qt6/QtCore",
        "-isystem/usr/include/x86_64-linux-gnu/qt6/QtQml",
        "-isystem/usr/include/x86_64-linux-gnu/qt6/QtQuick",
    ],
    "@platforms//cpu:aarch64": [
        "-isystem/usr/include/aarch64-linux-gnu/qt6",
        "-isystem/usr/include/aarch64-linux-gnu/qt6/QtGui",
        "-isystem/usr/include/aarch64-linux-gnu/qt6/QtWidgets",
        "-isystem/usr/include/aarch64-linux-gnu/qt6/QtCore",
        "-isystem/usr/include/aarch64-linux-gnu/qt6/QtQml",
        "-isystem/usr/include/aarch64-linux-gnu/qt6/QtQuick",
    ],
})

QT_LINK_PATHS = select({
    "@platforms//cpu:x86_64": [
        "-L/usr/lib/x86_64-linux-gnu",
        "-lQt6Widgets",
        "-lQt6Core",
        "-lQt6Gui",
        "-lQt6Qml",
        "-lQt6Quick",
    ],
    "@platforms//cpu:aarch64": [
        "-L/usr/lib/aarch64-linux-gnu",
        "-lQt6Widgets",
        "-lQt6Core",
        "-lQt6Gui",
        "-lQt6Qml",
        "-lQt6Quick",
    ],
})

qt_plugin_data = select({
    "@platforms//cpu:x86_64": ["/usr/lib/x86_64-linux-gnu", "/usr/lib/x86_64-linux-gnu/qt6/plugins", "/usr/lib/x86_64-linux-gnu/qt6/qml"],
    "@platforms//cpu:aarch64": ["/usr/lib/aarch64-linux-gnu", "/usr/lib/aarch64-linux-gnu/qt6/plugins", "/usr/lib/aarch64-linux-gnu/qt6/qml"],
})

x86_64_env = {
    "QT_PLUGIN_PATH": "/usr/lib/x86_64-linux-gnu/qt6/plugins",
    "QT_QPA_PLATFORM_PLUGIN_PATH": "/usr/lib/x86_64-linux-gnu/qt6/plugins/platforms",
    "QML2_IMPORT_PATH": "/usr/lib/x86_64-linux-gnu/qt6/qml",
}

aarch64_env = {
    "QT_PLUGIN_PATH": "/usr/lib/aarch64-linux-gnu/qt6/plugins",
    "QT_QPA_PLATFORM_PLUGIN_PATH": "/usr/lib/aarch64-linux-gnu/qt6/plugins/platforms",
    "QML2_IMPORT_PATH": "/usr/lib/aarch64-linux-gnu/qt6/qml",
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
        cmd = "echo 'Running uic command: /usr/lib/qt6/libexec/uic $(location %s) -o $@' && /usr/lib/qt6/libexec/uic $(location %s) -o $@" % (ui, ui),
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

def qt_cc_library(name, srcs, hdrs, features = [], copts = [], linkopts = [], deps = [], **kwargs):
    """
    Creates a cc_library for a Qt component.

    This rule processes Qt header files with the Qt Meta-Object Compiler (moc)
    and includes them in the library along with the provided sources.

    Args:
        name (str): The name of the library.
        srcs (list): A list of source files.
        hdrs (list): A list of header files to process with moc.
        features (list): A list of features to enable for this target.
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
            cmd = "echo 'Running moc command: /usr/lib/qt6/libexec/moc $(location %s) -o $@ -f \"%s\"' && /usr/lib/qt6/libexec/moc $(location %s) -o $@ -f \"%s\"" % (hdr, header_path, hdr, header_path),
        )

        _moc_srcs.append(":" + moc_name)

    cc_library(
        name = name,
        srcs = srcs + _moc_srcs,
        hdrs = hdrs,
        features = features,
        deps = deps,
        copts = copts + QT_INCLUDE_PATHS,
        linkopts = linkopts + QT_LINK_PATHS,
        **kwargs
    )

def qt_cc_binary(name, srcs, features = [], deps = None, copts = [], data = [], env = {}, **kwargs):
    """
    Creates a cc_binary for a Qt application.

    This rule generates a binary with the necessary environment variables and
    runtime paths for Qt libraries.

    Args:
        name (str): The name of the binary.
        srcs (list): A list of source files for the binary.
        features (list): A list of features to enable for this target.
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
        outs = [name + "_qt_env.ini"],
        cmd = select({
            "@platforms//cpu:x86_64": "echo $$\"LD_LIBRARY_PATH: /usr/lib/x86_64-linux-gnu\" > $@ \
                    $$\"\r\nQT_QPA_PLATFORM_PLUGIN_PATH: /usr/lib/x86_64-linux-gnu/qt6/plugins/platforms\" > $@ \
                    $$\"\r\nQML2_IMPORT_PATH: /usr/lib/x86_64-linux-gnu/qt6/qml\" > $@ \
                    $$\"\r\nQT_PLUGIN_PATH: /usr/lib/x86_64-linux-gnu/qt6/plugins\" > $@",
            "@platforms//cpu:aarch64": "echo $$\"LD_LIBRARY_PATH: /usr/lib/aarch64-linux-gnu\" > $@ \
                    $$\"\r\nQT_QPA_PLATFORM_PLUGIN_PATH: /usr/lib/aarch64-linux-gnu/qt6/plugins/platforms\" > $@ \
                    $$\"\r\nQML2_IMPORT_PATH: /usr/lib/aarch64-linux-gnu/qt6/qml\" > $@ \
                    $$\"\r\nQT_PLUGIN_PATH: /usr/lib/aarch64-linux-gnu/qt6/plugins\" > $@",
        }),
    )
    env_file.append(name + "_qt_env.ini")
    cc_binary(
        name = name,
        srcs = srcs,
        features = features,
        deps = deps,
        copts = copts + QT_INCLUDE_PATHS,
        data = env_file + data,
        env = select({
            "@platforms//cpu:x86_64": x86_64_env_data,
            "@platforms//cpu:aarch64": aarch64_env_data,
        }),
        **kwargs
    )

def _gencpp(ctx):
    resource_files = [(f, ctx.actions.declare_file(f.path)) for f in ctx.files.files]
    for target_file, output in resource_files:
        ctx.actions.symlink(
            output = output,
            target_file = target_file,
        )

    args = ["--name", ctx.attr.resource_name, "--output", ctx.outputs.cpp.path, ctx.file.qrc.path]
    ctx.actions.run(
        inputs = [resource for _, resource in resource_files] + [ctx.file.qrc],
        outputs = [ctx.outputs.cpp],
        arguments = args,
        executable = "/usr/lib/qt6/libexec/rcc",
    )
    return [OutputGroupInfo(cpp = depset([ctx.outputs.cpp]))]

gencpp = rule(
    implementation = _gencpp,
    attrs = {
        "resource_name": attr.string(),
        "files": attr.label_list(allow_files = True, mandatory = False),
        "qrc": attr.label(allow_single_file = True, mandatory = True),
        "cpp": attr.output(),
    },
)

# generate a qrc file that lists each of the input files.
def _genqrc(ctx):
    qrc_output = ctx.outputs.qrc
    qrc_content = "<RCC>\n  <qresource prefix=\\\"/\\\">"
    for f in ctx.files.files:
        qrc_content += "\n    <file>%s</file>" % f.path
    qrc_content += "\n  </qresource>\n</RCC>"
    cmd = ["echo", "\"%s\"" % qrc_content, ">", qrc_output.path]
    ctx.actions.run_shell(
        command = " ".join(cmd),
        outputs = [qrc_output],
    )
    return [OutputGroupInfo(qrc = depset([qrc_output]))]

genqrc = rule(
    implementation = _genqrc,
    attrs = {
        "files": attr.label_list(allow_files = True, mandatory = True),
        "qrc": attr.output(),
    },
)

def qt_resource(name, files, copts = [], linkopts = [], **kwargs):
    """Creates a cc_library containing the contents of all input files using qt's `rcc` tool.

    Args:
        name: rule name
        files: a list of files to be included in the resource bundle
        copts (list): Additional compiler options.
        linkopts (list): Additional linker options.
        **kwargs: Additional keyword arguments for `cc_library`.
    """
    qrc_file = name + "_qrc.qrc"
    genqrc(name = name + "_qrc", files = files, qrc = qrc_file)

    # every resource cc_library that is linked into the same binary needs a
    # unique 'name'.
    rsrc_name = native.package_name().replace("/", "_") + "_" + name

    outfile = name + "_gen.cpp"
    gencpp(
        name = name + "_gen",
        resource_name = rsrc_name,
        files = files,
        qrc = qrc_file,
        cpp = outfile,
    )
    cc_library(
        name = name,
        srcs = [outfile],
        alwayslink = 1,
        copts = copts + QT_INCLUDE_PATHS,
        linkopts = linkopts + QT_LINK_PATHS,
        **kwargs
    )
