#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>

#include "examples/qt/ui/ui_mainwindow.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   private slots:
    void on_pushButton_clicked();

   private:
    Ui::MainWindow *ui;
};

#endif  // MAINWINDOW_H
