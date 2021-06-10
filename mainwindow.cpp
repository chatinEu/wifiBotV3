#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qdebug.h"

#include "iostream"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{





    ui->setupUi(this);
    connect(&robot, SIGNAL(updateUI(QByteArray)),this, SLOT(updateGUI(QByteArray)));
    connect(robot.getSocket(), SIGNAL(connected()),this, SLOT(connectionLabelSlot()));
    connect(robot.getSocket(), SIGNAL(disconnected()),this, SLOT(disconnectionLabelSlot()));
    ui->progressBar->setRange(0,255);

    ui->progressIRFrontLeft->setRange(0,255);
    ui->progressIRFrontRight->setRange(0,255);
    ui->progressIRBackLeft->setRange(0,255);
    ui->progressIRBackRight->setRange(0,255);
}



void MainWindow::updateGUI(QByteArray arr)
{
    qDebug()<<"updating gui";
    ui->progressBar->setValue(robot.getBatteryLevel());
    ui->progressIRFrontLeft->setValue(robot.getLFIRLevel());
    ui->progressIRFrontRight->setValue(robot.getRFIRLevel());
    ui->progressIRBackLeft->setValue(robot.getLBIRLevel());
    ui->progressIRBackRight->setValue(robot.getRBIRLevel());

}

void MainWindow::on_BtnStop_clicked()
{
    robot.setForward(0);
}
void MainWindow::on_BtnForward_clicked(){
    robot.setForward(10);
}

void MainWindow::on_BtnReverse_clicked()
{
    robot.setReverse(110);
}


void MainWindow::on_BtnRight_clicked()
{
    robot.setRight(110);
}



void MainWindow::on_BtnLeft_clicked()
{
    robot.setLeft(110);
}



void setMainWindow(){

}


MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_BtnConnection_clicked()
{
    robot.doConnect();
}


void MainWindow::connectionLabelSlot()
{
    this->ui->label->setText("ONLINE");

   // disconnect(ui->BtnConnection,SIGNAL(clicked()),this,SLOT(on_BtnDisconnection_clicked()));
   // connect(ui->BtnConnection,SIGNAL(clicked()),this,SLOT(on_BtnConnection_clicked()));
}

void MainWindow::disconnectionLabelSlot()
{
    this->ui->label->setText("OFFLINE");
   // disconnect(ui->BtnConnection,SIGNAL(clicked()),this,SLOT(on_BtnConnection_clicked()));
   // connect(ui->BtnConnection,SIGNAL(clicked()),this,SLOT(on_BtnDisconnection_clicked()));
}

bool MainWindow::event(QEvent *event)
{
    bool bol=false;
    if (event->type() == QEvent::KeyRelease) {
            QKeyEvent *ke = static_cast<QKeyEvent *>(event);
            if (ke->key() == Qt::Key_Left) {
                qDebug()<<"pressed left";
                robot.setLeft(110);
            }
            if (ke->key() == Qt::Key_Right) {
                qDebug()<<"pressed right";
                robot.setRight(110);
            }
            if (ke->key() == Qt::Key_Down) {
                qDebug()<<"pressed down";
                robot.setReverse(110);
            }
            if (ke->key() == Qt::Key_Up) {
                qDebug()<<"pressed up";
                robot.setForward(110);
            }
    }
    return bol;
}



