#include <qapplication.h>

#include <QApplication>
#include <memory>

#include "hello.hpp"

auto main(int argc, char **argv) -> int {
    QApplication const app(argc, argv);

    const int WindowWidth = 600;
    const int WindowHeight = 600;

    auto mainWidget = std::make_unique<MyWidget>();
    mainWidget->resize(WindowWidth, WindowHeight);
    mainWidget->show();

    return QApplication::exec();
}
