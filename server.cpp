#include "server.h"
#include <QSerialPortInfo>

Server::Server(){
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()){
       comPorts.push_back(port.portName());
    }
    qDebug()<<"List of all porst:"<<endl;
    qDebug()<<comPorts<<endl;

}


Server::Server(int port){
    this->port = port;
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()){
       comPorts.push_back(port.portName());
    }
    qDebug()<<"List of all porst:"<<endl;
    qDebug()<<comPorts<<endl;
}


Server::~Server(){
    for(int i = 0; i < Arduinos.size(); ++i){
        qDebug()<<"Closing: "<<i<<endl;
        Arduinos[i]->close();
    }
    this->close();
}

void Server::sendComData(QString Data, QSerialPort *Com)
{
    Com->write(Data.toStdString().c_str());
}

void Server::openComPort(QString port)
{
    arduino = new QSerialPort(this);
    arduino->setPortName(port);
    arduino->setBaudRate(QSerialPort::Baud115200);
    arduino->setDataBits(QSerialPort::Data8);
    arduino->setParity(QSerialPort::NoParity);
    arduino->setStopBits(QSerialPort::OneStop);
    arduino->setFlowControl(QSerialPort::NoFlowControl);
    arduino->open(QSerialPort::WriteOnly);
    arduino->open(QIODevice::WriteOnly);

    Arduinos.push_back(arduino);
    qDebug()<<"OK"<<endl;

}

void Server::startServer(){
    //QTcpServer::setMaxPendingConnections(1);
    if(this->listen(QHostAddress::Any, port)){
        qDebug()<<"Listening\n";
    }
    else{
        qDebug()<<"Not listening\n";
    }
    qDebug()<<"Opening Com ports:"<<endl;
    for(const auto &port : qAsConst(comPorts)){
        qDebug()<<"Opening: "<<port<<endl;
        openComPort(port);
    }
    qDebug()<<"DONE"<<endl;
    qDebug()<<"Ad info:"<<endl;
    for(int i = 0; i < Arduinos.size(); ++i){
        Arduinos[i]->close();
        qDebug()<<Arduinos[i]<<endl;
    }

}

void Server::incomingConnection(qintptr handle){
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(handle);
    connect(socket,SIGNAL(readyRead()),this,SLOT(socketReady()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(socketDisconnected()));
    qDebug()<<"Connected! ID: "<<handle<<endl;
    socket->write("Success connection");
}

void Server::socketReady(){
    Data = socket->readAll();
    qDebug()<<Data<<endl;
}

void Server::socketDisconnected(){
    socket->deleteLater();
}
