#ifndef BASICDISPLAY_H
#define BASICDISPLAY_H

#include <QMainWindow>
#include "speed.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class BasicDisplay;
}
QT_END_NAMESPACE

class BasicDisplay : public QMainWindow
{
    Q_OBJECT

public:
    BasicDisplay(QWidget *parent = nullptr);
    ~BasicDisplay();
private slots:
    void simulate();
    void showSpeed(double speed);
    void on_accelerate_pressed();
    void on_decelerate_pressed();
private:
    Ui::BasicDisplay *ui;
    Speed speed = *new Speed(30);
};
#endif // BASICDISPLAY_H
