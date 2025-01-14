#include "basicdisplay.h"
#include "qwidget.h"
#include "qmainwindow.h"
#include "qobject.h"
#include "ui_basicdisplay.h"

BasicDisplay::BasicDisplay(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::BasicDisplay)
{
    ui->setupUi(this);
}

BasicDisplay::~BasicDisplay() {
    delete ui;
}

void BasicDisplay::updateSpeed(double speedValue) {
    speed.setSpeed(speedValue);
    ui->lcdNumber->display(QString::number(speed.getSpeed()));
}

void BasicDisplay::updateRPM(int rpmValue) {
    rpm.setRPM(rpmValue);
    ui->lcdRPM->display(QString::number(rpm.getRPM()));
}
