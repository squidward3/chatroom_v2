#include "client.h"

client::client(QWidget * parent):QWidget(parent),_port(0),_ip("114.55.249.200")
{
    connect(&_socket,&QTcpSocket::connected,this,[&](){
        QMessageBox qm;
        qm.setWindowTitle("成功");
        qm.setText("链接成功");
        qm.exec();
        emit success_conected();

    });

    connect(&_socket,&QTcpSocket::disconnected,this,[&](){
        QMessageBox qm;
        qm.setWindowTitle("失败");
        qm.setText("链接失败");
        qm.exec();
    });

    connect(&_socket,&QTcpSocket::readyRead,this,&client::_getmessage);
    // qDebug()<<"我在client中的第18行";
}

client::~client()
{
    ;
}

void client::tcp_connect()
{
    // qDebug()<<"准备链接";
    _socket.connectToHost(_ip,_port);

    // qDebug()<<"1";
}

void client::setport(int port)
{
    _port=port;
}

void client::sendTosever(QByteArray & message)
{
    // _socket.waitForConnected(3000);
    serialize(message);
    _socket.write(message.toStdString().c_str(),message.size());
}

void client::_getmessage()
{
    QByteArray _message  = _socket.readAll();
    qDebug()<<_message;
}



void client::serialize(QByteArray &message)
{
    message.insert(0,FLAG_FRONT);
    message.insert(message.size(),FLAG_END);
    //测试用代码
    qDebug()<<message;
}

void client::disserialize(QByteArray &message,QVector<QByteArray>* messages)
{
    size_t front, end, str_len;
    str_len = strlen(FLAG_END);
    end = 0;
    while (1)
    {
        front = message.indexOf(FLAG_FRONT, end);
        end = message.indexOf(FLAG_END, front);
        if (front == std::string::npos || end == std::string::npos)
            break;
        messages->push_back(message.mid(front + str_len, end - front - str_len));
    }
    //测试用代码
    client::showmessages();
}

void client::showmessages()
{
    for(auto e:_messages)
    {
        qDebug()<<e;
    }
}

//登入界面
void client::_register(QByteArray &content)
{
    sendTosever(content);
}

void client::_login(QByteArray &content) //to test
{
    sendTosever(content);
    QMessageBox mb;
    mb.setText("链接中......");
    mb.setWindowTitle("Loding");
    mb.exec();
    while(1)
    {
        _getmessage();
        if(_message.indexOf("/loginY")!=-1)
        {

            emit enter_main();//进入聊天界面
            break;
        }
        else
        {
            mb.close();
            QMessageBox fail;
            fail.setText("用户名或者密码错误");
            fail.setWindowTitle("登入失败");
        }
    }
}
