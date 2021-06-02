#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "myrobot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();
    void connectToRobot();

private slots:


    void on_pushButton_6_clicked();

    void on_BtnForward_clicked();

    void on_BtnLeft_clicked();

    void on_BtnReverse_clicked();

    void on_BtnRight_clicked();

private:
    Ui::MainWindow *ui;
    MyRobot robot ;
public slots:
    void connectionLabelSlot();
};
#endif // MAINWINDOW_H
