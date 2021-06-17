// myrobot.cpp

#include "myrobot.h"
#include <iostream>
#include <mainwindow.h>

#define _USE_MATH_DEFINES
#include "math.h"

MyRobot::MyRobot(QObject *parent) : QObject(parent) {

    int timerTimeout = 75;
    socket = new QTcpSocket(this); // socket creation

    DataToSend.resize(9);
    DataToSend[0] = 0xff;
    DataToSend[1] = 0x07;
    DataToSend[2] = 0x0;//left speed
    DataToSend[3] = 0x0;//left speed
    DataToSend[4] = 0x0;//right speed
    DataToSend[5] = 0x0;//right speed
    DataToSend[6] = 0x0;
    DataToSend[7] = 0x0;//CRC
    DataToSend[8] = 0x0;//CRC
    DataReceived.resize(21);
    TimerEnvoi = new QTimer();
    updated();
    // setup signal and slot
    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot())); //Send data to wifibot timer

    /*
     * bit 0 = relay sensor pin 3
     * bit 0 = relay sensor pin 4 et 5
     * bit 0 = relay sensor pin 11 et 12
     * bit 0 = relay sensor pin 13 et 14
     * bit 4 = right side 1 forward 0 reverse
     * bit 5
     * bit 6 = left side 1 forward 0 reverse
     * bit 7
     */
}





bool MyRobot::doConnect() {

    netService.doConnect();
}

QTcpSocket *MyRobot::getSocket()
{
    return netService.getSocket();
}



void MyRobot::updated()
{
    for (int i =0 ; i<9;i++){
        std::cout<<std::hex <<DataToSend[i]<<std::endl;
    }

    /*for(int i=0; i<20;i++){
        std::cout<< std::hex << DataReceived[i]<<std::endl;
    }*/
}








float MyRobot::parseBatteryLevel()
{
    unsigned char adc1=DataReceived[2];
    qDebug() <<"battery  level= "<<adc1;
    batteryLevel=adc1;
    return adc1;
}

float MyRobot::getBatteryLevel()
{
    return batteryLevel;
}

void MyRobot::parseIRLevels()
{
    LFIR = DataReceived[3];
    LBIR = DataReceived[4];

    RFIR = DataReceived[11];
    RBIR = DataReceived[12];
}


/** Infrared sensors */

float MyRobot::getRBIRLevel()
{
    return RBIR;
}

float MyRobot::getRightOdometer()
{
    return ROdo;
}
float MyRobot::getLeftOdometer()
{
    return LOdo;
}
float MyRobot::getRFIRLevel()
{
    return RFIR;
}
float MyRobot::getLBIRLevel()
{
    return LBIR;
}
float MyRobot::getLFIRLevel()
{
    return LFIR;
}



void MyRobot::setForward(int speed)
{
    netService.setForward(speed);

}

void MyRobot::setReverse(int speed)
{
    netService.setReverse(speed);
}

void MyRobot::setLeft(int speed)
{

    netService.setLeft(speed);
}

void MyRobot::setRight(int speed)
{
    netService.setRight(speed);
}

float MyRobot::getSpeed(){
    return speed;
}

NetworkService MyRobot::getNetService()
{

}








void MyRobot::parseReceivedData()
{
   parseBatteryLevel();
   parseIRLevels();
   parseOdometersValues();
}

void MyRobot::parseOdometersValues()
{
    float* odometers=netService.parseOdometersValues();
    LOdo= *odometers;
    ROdo= *(odometers+1);

    int timerTimeout = 75; //ms
    timerTimeout = timerTimeout * 10^-3;
    float tickDiff= ROdo- lastOdoTick;
    float rotorRotation=abs( tickDiff);
    float tailleRoues= 0.2; //en metres => 20cm
    float perimetre = 2* M_PI*tailleRoues;
    float distance = rotorRotation *perimetre;
    speed = distance/timerTimeout;
    lastOdoTick= ROdo;

}


