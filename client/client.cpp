#include "chatroom.h"

client::client():_port(0),_ip("0")
{
    connect(&_socket,&QTcpSocket::connected,this,[&](){
        QMessageBox qm;
        qm.setWindowTitle("成功");
        qm.setText("链接成功");
    });

    connect(&_socket,&QTcpSocket::disconnected,this,[&](){
        QMessageBox qm;
        qm.setWindowTitle("失败");
        qm.setText("链接失败");
    });
}

void client::tcp_connect()
{
    _socket.connectToHost(_ip,_port);
}

void client::setport(int port)
{
    _port=port;
}

client::~client()
{
    ;
}
