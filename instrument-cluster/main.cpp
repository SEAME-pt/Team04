#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>

#include "DataManager.hpp"

// #include "mq/ZMQSubscriber.hpp"

auto main(int argc, char *argv[]) -> int {
    QGuiApplication app(argc, argv);

    // ZmqSubscriber const subscriber("ipc:///tmp/speed.ipc");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

    QObject::connect(qApp, &QCoreApplication::aboutToQuit, [&]() {
        // The DataManager's destructor will handle cleanup.  No need to repeat it here.
        qDebug() << "Application is quitting";
    });

    qmlRegisterType<DataManager>("DataManager", 1, 0, "DataManager");
    engine.load(QUrl(QStringLiteral("qrc:/instrument-cluster/Main.qml")));

    return QGuiApplication::exec();
}
