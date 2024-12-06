#ifndef DISPLAY_H
#define DISPLAY_H

#include <QMainWindow>
#include "speed.h"

QT_BEGIN_NAMESPACE
namespace Ui {class Display;}
QT_END_NAMESPACE
class Display : public QMainWindow {
    Q_OBJECT
   public:
    Display(QWidget *parent = nullptr);
    ~Display();
   private slots:
    void simulate();
    void showSpeed(double speed);
    void on_accelerate_pressed();
    void on_decelerate_pressed();
   private:
    Ui::Display *ui;
    Speed speed = *new Speed(30);
};
#endif  // DISPLAY
