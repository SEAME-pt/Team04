#include "display.h"
#include "speed.h"
#include "./ui_display.h"
Display::Display(QWidget *parent) : QMainWindow(parent), ui(new Ui::Display) {
    ui->setupUi(this);
    simulate();
}
Display::~Display() { delete ui; }
void Display::showSpeed(double speed){
    ui->lcdNumber->display(QString::number(speed));
}
void Display::simulate(){
    showSpeed(speed.getCurrent());
}
void Display::on_accelerate_pressed()
{
    this->speed.increase();
    showSpeed(speed.getCurrent());
}
void Display::on_decelerate_pressed()
{
    this->speed.decrease();
    showSpeed(speed.getCurrent());
}
