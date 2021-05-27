#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "iostream"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    MyRobot robot= new MyRobot(this);
    ui->setupUi(this);
    //this->connectToRobot();
}

void MainWindow::init()
{
    //robot pas crée car appel dans le constructeur
    QTcpSocket* socket=robot.getSocket();
    connect(socket, SIGNAL(connected()),this, SLOT(connectionLabelSlot()));
}

void MainWindow::on_pushButton_clicked(){
    robot.setForward();
}

void MainWindow::connectToRobot(){
    robot.doConnect();
}



void setMainWindow(){

}














MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_2_clicked()
{
    robot.updated();
}


void MainWindow::on_pushButton_6_clicked()
{
    this->connectToRobot();
}

void MainWindow::connectionLabelSlot()
{
    std::cout<<"connected";
    this->ui->label->setText("ONLINE");
}

