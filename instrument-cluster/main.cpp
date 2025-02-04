#include <QtGui/QFontDatabase>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>

#include "src/DataManager.hpp"

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

    // adding qml folder to import path
    engine.addImportPath("qrc:/instrument-cluster/qml");

    // Registering C++ objects as QML types
    qmlRegisterType<DataManager>("DataManager", 1, 0, "DataManager");
    engine.load(QUrl(QStringLiteral("qrc:/instrument-cluster/Main.qml")));

    // Adding custom font
    // qint32 font_id =
    // QFontDatabase::addApplicationFont(":/instrument-cluster/fonts/HelveticaNeue.ttf");
    // QStringList font_list = QFontDatabase::applicationFontFamilies(font_id);
    // qDebug() << font_list;

    // Setting as default font
    // QString family = font_list.first();
    // QGuiApplication::setFont(QFont(family));

    return QGuiApplication::exec();
}
