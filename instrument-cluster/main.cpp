#include <QApplication>

#include "display.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Display w;
    w.show();
    return a.exec();
}
