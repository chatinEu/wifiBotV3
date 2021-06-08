#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "myrobot.h"
#include "keyboardfilter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void connectToRobot();
    void on_pushButton_clicked();
    MyRobot robot= MyRobot() ;

public slots:
    void connectionLabelSlot();
    void disconnectionLabelSlot();
    void updateGUI(QByteArray arr);
    void upArrow();

private slots:


    void on_BtnForward_clicked();

    void on_BtnLeft_clicked();

    void on_BtnReverse_clicked();

    void on_BtnRight_clicked();

    void on_BtnStop_clicked();

    void on_BtnConnection_clicked();

    void on_BtnDisconnection_clicked();

private:
    Ui::MainWindow *ui;
    KeyBoardFilter keyFilter=KeyBoardFilter(this);

};
#endif // MAINWINDOW_H
