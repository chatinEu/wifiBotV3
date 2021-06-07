// myrobot.cpp

#include "myrobot.h"
#include <iostream>
#include <mainwindow.h>

MyRobot::MyRobot(QObject *parent) : QObject(parent) {




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





void MyRobot::doConnect() {
    socket = new QTcpSocket(this); // socket creation
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));

    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    qDebug() << "connecting..."; // this is not blocking call
    //socket->connectToHost("LOCALHOST", 15020);
    socket->connectToHost("192.168.1.11", 15020); // connection to wifibot

    // we need to wait...
    if(!socket->waitForConnected(5000)) {
        qDebug() << "Error: " << socket->errorString();
        return;
    }
    TimerEnvoi->start(75);

}

void MyRobot::disConnect() {
    TimerEnvoi->stop();
    socket->close();
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



short MyRobot::Crc16(unsigned char *_Adresse_tab, unsigned char Taille_Max){
    unsigned int Crc = 0xFFFF;
    unsigned int Polynome = 0xA001;
    unsigned int CptOctet = 0;
    unsigned int CptBit = 0;
    unsigned int Parity = 0;

    Crc = 0xFFFF;
    Polynome = 0xA001;
    for (CptOctet=0; CptOctet<Taille_Max;CptOctet++){
        Crc ^= *(_Adresse_tab + CptOctet);

        for(CptBit=0;CptBit<=7;CptBit++){
            Parity = Crc;
            Crc >>=1;
            if(Parity%2 == true) Crc ^= Polynome;
        }
    }
    return (Crc);
}

QTcpSocket *MyRobot::getSocket()
{
    return socket;
}






float MyRobot::getBatteryPercent()
{
    unsigned char adc1=DataReceived[2];
    unsigned char adc2=DataReceived[4];
    unsigned char adc3=DataReceived[3];
    adc1<<adc1;
    adc2<<adc2;
    adc3<<adc3;

    unsigned char  adc=adc1+adc2+adc3;

    qDebug() <<"battery  level= "<<adc1;
    qDebug() <<"battery  level= "<<adc2;
    qDebug() <<"battery  level= "<<adc3;

    qDebug() <<"battery  level= "<<adc;


    return adc/255*100;
}

void MyRobot::setForward(int speed)
{
    //TODO: set  loopback
    setWheelSpeed( speed);
    DataToSend[6] = (unsigned char)80;      //0xf0;
    updateCrc();

}

void MyRobot::setReverse(int speed)
{
    setWheelSpeed( speed);
    DataToSend[6] = (unsigned char)0;
    updateCrc();
}

void MyRobot::setLeft(int speed)
{
    // TODO: char 6
    setWheelSpeed( speed);
//    DataToSend[6] = (unsigned char)0;
    DataToSend[4] = (unsigned char)0;
    DataToSend[5] = (unsigned char)0;
    updateCrc();
}

void MyRobot::setRight(int speed)
{
    // TODO: char 6
    setWheelSpeed( speed);
    DataToSend[2] = (unsigned char)0;
    DataToSend[3] = (unsigned char)0;
    updateCrc();
}



void MyRobot::setForward(){
    short speed1 = 120;
    short speed2 = 120;
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = (unsigned char) speed1;
    DataToSend[3] = (unsigned char)(speed1 >> 8);
    DataToSend[4] = (unsigned char) speed2;
    DataToSend[5] = (unsigned char)(speed2 >> 8);
    DataToSend[6] = (unsigned char)(80);
    short mycrcsend = Crc16((unsigned char *)DataToSend.data()+1,6);
    DataToSend[7] = (unsigned char) mycrcsend;
    DataToSend[8] = (unsigned char) (mycrcsend >> 8);

    //connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot()));
    updated();
}


void MyRobot::connected() {
    qDebug() << "connected..."; // Hey server, tell me about you.


}

void MyRobot::disconnected() {
    qDebug() << "disconnected...";
}

void MyRobot::bytesWritten(qint64 bytes) {
    updated();
    qDebug() << bytes << " bytes written...";
}

void MyRobot::readyRead() {
    qDebug() << "reading..."; // read the data from the socket
    DataReceived = socket->readAll();
    parseReceivedData();
    emit updateUI(DataReceived);
    qDebug() << DataReceived[0] << DataReceived[1] << DataReceived[2];
}

void MyRobot::MyTimerSlot() {
    qDebug() << "Timer...";
    while(Mutex.tryLock());
    socket->write(DataToSend);
    Mutex.unlock();
}

void MyRobot::updateCrc()
{
    short crc = Crc16((unsigned char *)DataToSend.data()+1,6);
    DataToSend[7] = (unsigned char)(crc >> 0);
    DataToSend[8] = (unsigned char)(crc >> 8);
}

void MyRobot::setWheelSpeed(short speed)
{

    DataToSend[2] = (unsigned char)speed;   //0x78; //left speed
    DataToSend[3] = (unsigned char)speed;           //left speed
    DataToSend[4] = (unsigned char)speed;   //0x78; //right speed
    DataToSend[5] = (unsigned char)speed;           //right speed
}

void MyRobot::parseReceivedData()
{
    getBatteryPercent();
}


