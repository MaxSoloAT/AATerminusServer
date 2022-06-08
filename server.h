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
    void sendComData(QString Data, QSerialPort *Com);   //Convert bytes to string to char and send to comport

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
    void startServer(); //Starts TCP server
    void incomingConnection(qintptr socketDescriptor);  //Calls when new client connected successfully
    void socketReady(); //Read data that cames from client and pars
    void socketDisconnected();  //call after client disconnected and delete socket
};

#endif // SERVER_H
