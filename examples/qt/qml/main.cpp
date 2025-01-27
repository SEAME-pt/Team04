#include <QtCore/QString>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtWidgets/QApplication>

auto main(int argc, char *argv[]) -> int {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QString button_text = "Click me!";
    engine.rootContext()->setContextProperty("buttonText", button_text);

    engine.load(QUrl(QStringLiteral("qrc:/examples/qt/qml/main.qml")));

    return QGuiApplication::exec();
}
