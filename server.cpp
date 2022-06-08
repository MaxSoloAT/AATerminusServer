#include "server.h"
#include <QSerialPortInfo>

Server::Server(){
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()){
       if(port.portName() != "ttyS0" || port.portName() != "ttyS1"){
       comPorts.push_back(port.portName());
       }

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

//Convert bytes to string to char and send to comport
void Server::sendComData(QString Data, QSerialPort *Com)
{
    Data = Data + "\r";
    Com->write(Data.toStdString().c_str());
    qDebug()<<"COM: "<<Com<<endl;
    qDebug()<<"Data: "<<Data.toStdString().c_str()<<endl;
    qDebug()<<"-------------------------------------------"<<endl;

}

//Create a new comport poiter and setting up the comport(special for Arduino Uno),
//push pointer to vector(list)
void Server::openComPort(QString port)
{
    qDebug()<<"\tConfigured new COM...";
    arduino = new QSerialPort(this);
    arduino->setPortName(port);
    arduino->setBaudRate(QSerialPort::Baud115200);
    arduino->setDataBits(QSerialPort::Data8);
    arduino->setParity(QSerialPort::NoParity);
    arduino->setStopBits(QSerialPort::OneStop);
    arduino->setFlowControl(QSerialPort::NoFlowControl);
    arduino->open(QSerialPort::WriteOnly);
    arduino->open(QIODevice::WriteOnly);
    qDebug()<<arduino->isOpen();
    Arduinos.push_back(arduino);
    qDebug()<<"\tDONE"<<endl;

}

//Starts TCP server
void Server::startServer(){
    //todo: enable only 1 connection
    //QTcpServer::setMaxPendingConnections(1);
    if(this->listen(QHostAddress::Any, port)){
        qDebug()<<"\t\t\t\t\tServer started\n";
    }
    else{
        qDebug()<<"\t\t\t\t\tError\n";
    }
    qDebug()<<"Opening COM ports:"<<endl;
    for(const auto &port : qAsConst(comPorts)){
        qDebug()<<"\tCOM: "<<port<<endl;
        openComPort(port);
    }
    qDebug()<<"DONE"<<endl;
    qDebug()<<"Additional information:"<<endl;
    for(int i = 0; i < Arduinos.size(); ++i){
        //Arduinos[i]->close(); //wait wat?
        qDebug()<<Arduinos[i];
    }

}
//Calls when new client connected successfully
void Server::incomingConnection(qintptr handle){
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(handle);
    connect(socket,SIGNAL(readyRead()),this,SLOT(socketReady()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(socketDisconnected()));
    qDebug()<<"New client connected! ID: "<<handle<<endl;
    socket->write("Success connection");
}
//Read data that cames from client and pars
void Server::socketReady(){
    Data = socket->readAll();
    QStringList list = QString(Data).split("|");
    qDebug()<<"INCOMMING DATA"<<endl;
    qDebug()<<list<<endl;

    //todo: add paralel IF it's needed...
    for(int i = 0; i < list.size();++i){
        sendComData(list[i],Arduinos[i]);
    }
}
//call after client disconnected and delete socket
void Server::socketDisconnected(){
    socket->deleteLater();
}
