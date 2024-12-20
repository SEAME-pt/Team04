#ifndef BASICDISPLAY_H
#define BASICDISPLAY_H

#include <QMainWindow>
#include "speed.h"
#include "rpm.h"

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
    void updateSpeed(double speed);
    void updateRPM(int rpm);

   private:
    Ui::BasicDisplay *ui;
    Speed speed;
    Rpm rpm;

};

#endif
