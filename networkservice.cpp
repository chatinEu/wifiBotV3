#include "networkservice.h"

NetworkService::NetworkService(QObject *parent) : QObject(parent) {

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


bool NetworkService::doConnect() {

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
        return false;
    }
    TimerEnvoi->start(75);
    return true;

}

void NetworkService::disConnect() {
    TimerEnvoi->stop();
    socket->close();
}


void NetworkService::connected() {
    qDebug() << "connected..."; // Hey server, tell me about you.
}

void NetworkService::disconnected() {
    qDebug() << "disconnected...";
}

void NetworkService::bytesWritten(qint64 bytes) {
    qDebug() << bytes << " bytes written...";
}

void NetworkService::readyRead() {
    qDebug() << "reading..."; // read the data from the socket
    DataReceived = socket->readAll();
    emit updateUI(DataReceived);
    //qDebug() << DataReceived[0] << DataReceived[1] << DataReceived[2];
}

void NetworkService::MyTimerSlot() {
    qDebug() << "Timer...";
    while(Mutex.tryLock());
    socket->write(DataToSend);
    Mutex.unlock();
}




short NetworkService::Crc16(unsigned char *_Adresse_tab, unsigned char Taille_Max){
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




void NetworkService::updateCrc()
{
    short crc = Crc16((unsigned char *)DataToSend.data()+1,6);
    DataToSend[7] = (unsigned char)(crc >> 0);
    DataToSend[8] = (unsigned char)(crc >> 8);
}



void NetworkService::setForward(int speed)
{
    setWheelSpeed( speed,speed);
    DataToSend[6] = (unsigned char)80;      //0xf0;
    updateCrc();

}

void NetworkService::setReverse(int speed)
{
    setWheelSpeed( speed,speed);
    DataToSend[6] = (unsigned char)0;
    updateCrc();
}

void NetworkService::setLeft(int speed)
{

    setWheelSpeed( 0,speed);
    DataToSend[6] = (unsigned char)80;
    updateCrc();
}

void NetworkService::setRight(int speed)
{
    setWheelSpeed( speed,0);
    DataToSend[6] = (unsigned char)80;
    updateCrc();
}



QTcpSocket *NetworkService::getSocket()
{
    return socket;
}

float* NetworkService::parseOdometersValues()
{
    float LOdo=(((long) DataReceived[8]<<24))+(((long)DataReceived[7]<<16))+
            (((long)DataReceived[6]<<8))+((long)DataReceived[5]);

    float ROdo=(((long) DataReceived[16]<<24))+(((long)DataReceived[15]<<16))+
            (((long)DataReceived[14]<<8))+((long)DataReceived[13]);
    float res[2]={LOdo,ROdo};
    return res;
}
