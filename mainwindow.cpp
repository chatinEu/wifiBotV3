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
    connect(&keyFilter,&KeyBoardFilter::up_arrow,this,&MainWindow::upArrow);
    ui->progressBar->setRange(0,255);
}

void MainWindow::init()
{
    //robot pas crée car appel dans le constructeur
    QObject::connect(robot.getSocket(),SIGNAL(connected()),
            this, SLOT(connectionLabelSlot()));

   // connect(&robot,&MyRobot::updateUI,
    //        this,&MainWindow::updateGUI);







}

void MainWindow::updateGUI(QByteArray arr)
{
    ui->progressBar->setValue(robot.getBatteryLevel());

}

void MainWindow::upArrow()
{
    std::cout<<"heyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy"<<std::endl;
    robot.setForward(100);
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

void MainWindow::on_BtnDisconnection_clicked()
{

    robot.disConnect();
}


void MainWindow::connectionLabelSlot()
{
    this->ui->label->setText("ONLINE");

    disconnect(ui->BtnConnection,SIGNAL(clicked()),this,SLOT(on_BtnDisconnection_clicked()));
    connect(ui->BtnConnection,SIGNAL(clicked()),this,SLOT(on_BtnConnection_clicked()));
}

void MainWindow::disconnectionLabelSlot()
{
    this->ui->label->setText("OFFLINE");
    disconnect(ui->BtnConnection,SIGNAL(clicked()),this,SLOT(on_BtnConnection_clicked()));
    connect(ui->BtnConnection,SIGNAL(clicked()),this,SLOT(on_BtnDisconnection_clicked()));
}





