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
        std::cout<<"send"<<DataToSend[i]<<std::endl;
    }
}

//short MyRobot::Crc16(unsigned char *adresseDataToSend, unsigned char tailleMax)
//short MyRobot::generateCrc(unsigned char *adresseDataToSend, unsigned char tailleMax)
//{
//    unsigned int crc= 0xFFFF;
//    unsigned int polynome= 0xA001;
//    unsigned int cptOctet = 0;
//    unsigned int cptBit= 0;
//    unsigned int parity = 0;
//    crc = 0xFFFF;

//    polynome = 0xA001;
//    for( cptOctet=0;cptOctet<tailleMax; cptOctet++){
//        crc^= * (adresseDataToSend + cptOctet);
//        for( cptBit = 0; cptBit<=7;cptBit++){
//            parity= crc;
//            crc>>=1;
//            if(parity%2 == true) crc ^=polynome;
//        }
//    }

//    std::cout<<"crc is: "<<crc<<std::endl;
//    return crc;
//}

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

void MyRobot::setForward(){
//    DataToSend.resize(9);
//    unsigned char DataToSend[10];
    short speed1 = 120;
    short speed2 = 120;
//    DataToSend[0]= 0xFF;
//    DataToSend[1]= 0x07;
//    DataToSend[2]= (unsigned char)speed1;
//    DataToSend[3]= (unsigned char)(speed1 >> 8);
//    DataToSend[4]= (unsigned char)speed2;
//    DataToSend[5]= (unsigned char)(speed2 >> 8);
//    DataToSend[6]= 80+1;
//    short mycrcsend = Crc16(sbuf+1,6);
//    short crc = generateCrc((unsigned char*)DataToSend.data()+1, 6);
//    DataToSend[7]= (unsigned char)crc;
//    DataToSend[8]= (unsigned char)(crc >> 8);
//    DataReceived.resize(21);
//    updated();
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = (unsigned char) speed1;
    DataToSend[3] = (unsigned char)(speed1 >> 8);
    DataToSend[4] = (unsigned char) speed2;
    DataToSend[5] = (unsigned char)(speed2 >> 8);
    DataToSend[6] = (unsigned char)(80);
//    short crc = generateCrc((unsigned char *)DataToSend.data()+1,6);
    short mycrcsend = Crc16((unsigned char *)DataToSend.data()+1,6);
    DataToSend[7] = (unsigned char) mycrcsend;
    DataToSend[8] = (unsigned char) (mycrcsend >> 8);

    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot()));
    updated();
}


//void MyRobot::setForward()
//{
//    short crc = generateCrc((unsigned char*)DataToSend.data()+1, 6);
//    DataToSend[0] = (unsigned char)255;
//    DataToSend[1] = (unsigned char)0x07;
//    DataToSend[2] = (unsigned char)120;//0x78;       //left speed
//    DataToSend[3] = (unsigned char)0x0;        //left speed
//    DataToSend[4] = (unsigned char)120;//0x78;       //right speed
//    DataToSend[5] = (unsigned char)0x0;       //right speed
//    DataToSend[6] = (unsigned char)0x00;//0xf0;
//    DataToSend[7] = (unsigned char)(crc >> 0);
//    DataToSend[8] = (unsigned char)(crc >> 8);

//    updated();
//    qDebug() << "sent";
//}

//--------------------------------------------------------------------------------------------------------TEST----------------

//void MyRobot::setForward()
//{	//Envoi la consigne pour le robot, à vérifier
//    unsigned char sendBuffer[30];
//    unsigned char flag = 0;
//    sendBuffer[0] = 255;
//    sendBuffer[1] = 0x07;

//    unsigned char droite = (unsigned char)1;
//    unsigned char gauche = (unsigned char)1;

//    int tmp1 = 8 * (droite & 0x3F);
//    int tmp2 = 8 * (gauche & 0x3F);

//    if (gauche & 0x80)
//        flag += 32;

//    if (gauche & 0x40)
//        flag += 16;

//    sendBuffer[2] = (unsigned)tmp1;
//    sendBuffer[3] = (unsigned)(tmp1 >> 8);
//    sendBuffer[4] = (unsigned)tmp2;
//    sendBuffer[5] = (unsigned)(tmp2 >> 8);
//    sendBuffer[6] = (droite & 0x80) + (droite & 0x40) + flag + 0 + 2 + 8 + 1;//+1 Relay ON +8 10ms pid mode ;

//    /*
//    Char 7 is decomposed as follow (1 byte char -> 8 bits):
//    (128) Bit 7 Left Side Closed Loop Speed control :: 1 -> ON / 0 -> OFF
//    (64) Bit 6 Left Side Forward / Backward speed flag :: 1 -> Forward / 0 -> Reverse
//    (32) Bit 5 Right Side Closed Loop Speed control :: 1 -> ON / 0 -> OFF
//    (16) Bit 4 Right Side Forward / Backward speed flag :: 1 -> Forward / 0 -> Reverse
//    (8) Bit 3 Relay 4 On/Off (DSUB15 POWER Pin 13 and 14)
//    (4) Bit 2 Relay 3 On/Off (DSUB15 POWER Pin 11 and 12)
//    (2) Bit 1 Relay 2 On/Off (DSUB15 POWER Pin 4 and 5)
//    (1) Bit 0 Relay 1 for Sensors. On/Off: 0 is OFF 1 is ON (DSUB15 POWER Pin 3)
//    */

//    short crc = generateCrc((unsigned char*)sendBuffer + 1, 6);

//    sendBuffer[7] = (unsigned)crc;
//    sendBuffer[8] = (unsigned)(crc >> 8);
//    TimerEnvoi = new QTimer();
//    updated();
//    // setup signal and slot
//    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot()));
//    // setup signal and slot
//   if (!socketnotok)
//        send(socktcp, (char*)sendBuffer, 9, 0);		// tester avec 8, 0  si ça marche pas

//}

//short MyRobot::generateCrc(unsigned char *Adresse_tab, unsigned char Taille_max)
//{
//    unsigned int Crc = 0xFFFF;
//    unsigned int Polynome = 0xA001;
//    unsigned int CptOctet = 0;
//    unsigned int CptBit = 0;
//    unsigned int Parity = 0;
//    Crc = 0xFFFF;
//    Polynome = 0xA001;
//    for (CptOctet = 0; CptOctet < Taille_max; CptOctet++)
//    {
//        Crc ^= *(Adresse_tab + CptOctet);
//        for (CptBit = 0; CptBit <= 7; CptBit++)
//        {
//            Parity = Crc;
//            Crc >>= 1;
//            if (Parity % 2 == 1) Crc ^= Polynome;
//        }
//    }
//    return(Crc);
//}

//--------------------------------------------------------------------------------------------------------TEST-----------------

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

