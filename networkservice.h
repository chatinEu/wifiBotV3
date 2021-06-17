#ifndef NETWORKSERVICE_H
#define NETWORKSERVICE_H


#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QTimer>
#include <QMutex>

class NetworkService : public QObject
{
    Q_OBJECT
public:
    NetworkService(QObject *parent = 0);
    bool doConnect();
    void disConnect();
    QByteArray DataToSend;
    QByteArray DataReceived;
    QMutex Mutex;
    QTcpSocket* getSocket();
    short Crc16(unsigned char *_Adresse_tab, unsigned char Taille_Max);
signals:
    void updateUI(const QByteArray Data);
public slots:
    void connected();
    void disconnected();
    void updateCrc();
    void setWheelSpeed(short lSpeed,short rSpeed);
    void bytesWritten(qint64 bytes);
    void readyRead();
    void MyTimerSlot();
    float* parseOdometersValues();

    void setForward(int speed);
    void setReverse(int speed);
    void setLeft(int speed);
    void setRight(int speed);
private:
    QTcpSocket *socket;
    QTimer *TimerEnvoi;
};

#endif // NETWORKSERVICE_H
