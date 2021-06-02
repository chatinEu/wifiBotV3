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
    void doConnect();
    void disConnect();
    QByteArray DataToSend;
    QByteArray DataReceived;
    QMutex Mutex;
    void updated();
//    short generateCrc(unsigned char *adresseDataToSend,unsigned char tailleMax);
    short Crc16(unsigned char *_Adresse_tab, unsigned char Taille_Max);
    QTcpSocket* getSocket();

signals:
    void updateUI(const QByteArray Data);
public slots:
    void connected();
    void disconnected();
    void setForward();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void MyTimerSlot();

private:
    QTcpSocket *socket;
    QTimer *TimerEnvoi;
};

#endif // MYROBOT_H
