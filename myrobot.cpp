// myrobot.cpp

#include "myrobot.h"
#include <iostream>

MyRobot::MyRobot(QObject *parent) : QObject(parent) {
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = 0x0;//left speed
    DataToSend[3] = 0x0;//left speed
    DataToSend[4] = 0x0;//right speed
    DataToSend[5] = 0x0;//right speed

    DataToSend[6] = 0x0;
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


    DataToSend[7] = 0x0;//CRC
    DataToSend[8] = 0x0;//CRC
    DataReceived.resize(21);
    TimerEnvoi = new QTimer();
    // setup signal and slot
    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot())); //Send data to wifibot timer
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
        std::cout<<DataToSend[i]<<std::endl;
    }
}

short MyRobot::getCrc(unsigned char *adresseDataToSend, unsigned char tailleMax)
{
    int crc= 0xFFFF;
    int polynome= 0xA001;
    int cptOctet = 0;
    int cptBit= 0;
    int parity = 0;
    crc = 0xFFFF;

    polynome = 0xA001;
    for( cptOctet=0;cptOctet<tailleMax; cptOctet++){
        crc^= * (adresseDataToSend + cptOctet);
        for( cptBit = 0; cptBit<=7;cptBit++){
            parity= crc;
            crc>>=1;
            if(parity%2 == true) crc ^=polynome;
        }
    }
    return crc;
}

void MyRobot::setForward()
{
    std::cout<<"sa marche"<<std::endl;
    DataToSend[2] = 0x78;//left speed
    DataToSend[3] = 0x0;//left speed
    DataToSend[4] = 0x78;//right speed
    DataToSend[5] = 0x0;//right speed
    DataToSend[6] = 0x80;


    short crc=getCrc(*DataToSend,9);
    DataToSend[7] = 0x0;//CRC
    DataToSend[8] = 0x0;//CRC
    updated();
}

void MyRobot::connected() {
    qDebug() << "connected..."; // Hey server, tell me about you.
}

void MyRobot::disconnected() {
    qDebug() << "disconnected...";
}

void MyRobot::bytesWritten(qint64 bytes) {
    qDebug() << bytes << " bytes written...";
}

void MyRobot::readyRead() {
    qDebug() << "reading..."; // read the data from the socket
    DataReceived = socket->readAll();
    emit updateUI(DataReceived);
    qDebug() << DataReceived[0] << DataReceived[1] << DataReceived[2];
}

void MyRobot::MyTimerSlot() {
    qDebug() << "Timer...";
    while(Mutex.tryLock());
    socket->write(DataToSend);
    Mutex.unlock();
}

