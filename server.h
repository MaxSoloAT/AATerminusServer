#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QSerialPort>
#include <QVector>
class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
    Server(int port);
    ~Server();
    void setPort();
    void sendComData(QString Data, QSerialPort *Com);

private:
    QByteArray Data;
    QTcpSocket* socket = nullptr;
    int port = 1331;
    QSerialPort *arduino;
    static const quint16 vendor_id = 9025;
    static const quint16 product_id = 67;
    QVector<QString> comPorts;
    QVector<QSerialPort*> Arduinos;
    void openComPort(QString port);


public slots:
    void startServer();
    void incomingConnection(qintptr socketDescriptor);
    void socketReady();
    void socketDisconnected();
};

#endif // SERVER_H
