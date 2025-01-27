#include <QtWidgets/QApplication>

#include "mainwindow.h"

auto main(int argc, char *argv[]) -> int {
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
