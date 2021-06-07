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
    void on_pushButton_clicked();
    MyRobot robot= MyRobot() ;

private slots:


    void on_pushButton_6_clicked();

    void on_BtnForward_clicked();

    void on_BtnLeft_clicked();

    void on_BtnReverse_clicked();

    void on_BtnRight_clicked();

    void on_BtnStop_clicked();

private:
    Ui::MainWindow *ui;

public slots:
    void connectionLabelSlot();
    void updateGUI(QByteArray);
};
#endif // MAINWINDOW_H
