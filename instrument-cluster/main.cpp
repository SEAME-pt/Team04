#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>

#include "mq/ZMQSubscriber.hpp"

auto main(int argc, char *argv[]) -> int {
    QGuiApplication app(argc, argv);

    ZmqSubscriber const subscriber("ipc:///tmp/speed.ipc");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.load(QUrl(QStringLiteral("qrc:/instrument-cluster/Main.qml")));

    return QGuiApplication::exec();
}
