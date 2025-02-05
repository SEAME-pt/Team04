#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <csignal>

#include "src/DataManager.hpp"

void sigHandler(int s) {
    std::signal(s, SIG_DFL);
    qApp->quit();
}

auto main(int argc, char *argv[]) -> int {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

    QObject::connect(qApp, &QCoreApplication::aboutToQuit, [&]() {
        // The DataManager's destructor will handle cleanup.  No need to repeat it here.
        qDebug() << "Application is quitting";
    });

    // adding qml folder to import path
    engine.addImportPath("qrc:/instrument-cluster/qml");

    // Registering C++ objects as QML types
    qmlRegisterType<DataManager>("DataManager", 1, 0, "DataManager");
    engine.load(QUrl(QStringLiteral("qrc:/instrument-cluster/Main.qml")));

    std::signal(SIGINT, sigHandler);
    std::signal(SIGTERM, sigHandler);
    std::signal(SIGSEGV, sigHandler);
    std::signal(SIGABRT, sigHandler);
    return QGuiApplication::exec();
}
