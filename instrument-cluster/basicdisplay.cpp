#include "basicdisplay.h"
#include "speed.h"
#include "ui_basicdisplay.h"

BasicDisplay::BasicDisplay(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BasicDisplay)
{
    ui->setupUi(this);
    simulate();
}
BasicDisplay::~BasicDisplay()
{
    delete ui;
}
void BasicDisplay::showSpeed(double speed){
    ui->lcdNumber->display(QString::number(speed));
}
void BasicDisplay::simulate(){
    showSpeed(speed.getCurrent());
}
void BasicDisplay::on_accelerate_pressed()
{
    this->speed.increase();
    showSpeed(speed.getCurrent());
}
void BasicDisplay::on_decelerate_pressed()
{
    this->speed.decrease();
    showSpeed(speed.getCurrent());
}
