#ifndef MYROBOT_H
#define MYROBOT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QTimer>
#include <QMutex>

class MyRobot : public QObject {
    Q_OBJECT
public:
    MyRobot(QObject *parent = 0);
    bool doConnect();
    void disConnect();
    QByteArray DataToSend;
    QByteArray DataReceived;
    QMutex Mutex;
    void updated();
//    short generateCrc(unsigned char *adresseDataToSend,unsigned char tailleMax);
    short Crc16(unsigned char *_Adresse_tab, unsigned char Taille_Max);
    QTcpSocket* getSocket();
    float getBatteryPercent();
    float getBatteryLevel();



signals:
    void updateUI(const QByteArray Data);
public slots:
    void setForward(int speed);
    void setReverse(int speed);
    void setLeft(int speed);
    void setRight(int speed);
    void connected();
    void disconnected();

    void bytesWritten(qint64 bytes);
    void readyRead();
    void MyTimerSlot();

private:
    QTcpSocket *socket;
    QTimer *TimerEnvoi;
    void updateCrc();
    void setWheelSpeed(short speed);
    void setWheelSpeed(short lSpeed,short rSpeed);
    void parseReceivedData(); //useless for now
    unsigned char bitwiseAddition(unsigned char c1,unsigned char c2);
    float batteryLevel;

};

#endif // MYROBOT_H
