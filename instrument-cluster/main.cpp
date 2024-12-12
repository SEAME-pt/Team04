#include "basicdisplay.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BasicDisplay w;
    w.show();
    return a.exec();
}
