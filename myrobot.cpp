// myrobot.cpp

#include "myrobot.h"
#include <iostream>


MyRobot::MyRobot(QObject *parent) : QObject(parent) {



    DataToSend.resize(9);
    DataToSend[0] = 0xff;
    DataToSend[1] = 0x07;
    DataToSend[2] = 0x0;//left speed
    DataToSend[3] = 0x0;//left speed
    DataToSend[4] = 0x0;//right speed
    DataToSend[5] = 0x0;//right speed

    DataToSend[6] = 0x00;
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
    //connect(socket, SIGNAL(connected()),this, MainWindow::SLOT(connectionLabelSlot()));

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

short MyRobot::generateCrc(unsigned char *adresseDataToSend, unsigned char tailleMax)
{int crc = 0xFFFF;
    int polynome = 0xA001;
    int cptOctet = 0;
    int cptBit = 0;
    int parity = 0;

    for (cptOctet = 0; cptOctet < tailleMax ; cptOctet++)
      {
        crc ^= *(adresseDataToSend + cptOctet);

        for (cptBit = 0; cptBit <= 7 ; cptBit++)
    {
      parity = crc;
      crc >>= 1;
      if (parity % 2 == true) crc ^= polynome;
    }
      }

    /*
    int crc= 0xFFFF;
    int polynome= 0xA001;
    int cptOctet = 0;
    int cptBit= 0;
    int parity = 0;


    for( cptOctet=0; cptOctet<tailleMax; cptOctet++){
        crc^= * (adresseDataToSend + cptOctet);
        for( cptBit = 0; cptBit<=7; cptBit++){
            parity= crc;
            crc>>=1;
            if(parity%2 == true) crc ^=polynome;
        }
    }
*/
    std::cout<<"crc is: "<<crc<<std::endl;
    return (crc);
}

QTcpSocket *MyRobot::getSocket()
{
    return socket;
}

void MyRobot::setForward()
{
    DataToSend[2] = (unsigned char)110;//0x78;       //left speed
    DataToSend[3] = (unsigned char)110;        //left speed
    DataToSend[4] = (unsigned char)110;//0x78;       //right speed
    DataToSend[5] = (unsigned char)110;       //right speed
    DataToSend[6] = (unsigned char)80;//0xf0;





    //crc not used in tcp ? probs true

    short crc = generateCrc((unsigned char*)DataToSend.data()+1, 6);
    DataToSend[7] = (unsigned char)(crc >> 0);
    DataToSend[8] = (unsigned char)(crc >> 8);
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
    emit updateUI(DataReceived);
    qDebug() << DataReceived[0] << DataReceived[1] << DataReceived[2];
}

void MyRobot::MyTimerSlot() {
    qDebug() << "Timer...";
    while(Mutex.tryLock());
    socket->write(DataToSend);
    Mutex.unlock();
}

