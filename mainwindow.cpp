#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->init();
    this->connect();
}



void MainWindow::on_pushButton_clicked()
{
    robot.setForward();
   //robot.setForward();
}

void MainWindow::connect(){
    robot.doConnect();
}



void setMainWindow(){

}














MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{

}

void MainWindow::on_pushButton_2_clicked()
{
    robot.updated();
}

