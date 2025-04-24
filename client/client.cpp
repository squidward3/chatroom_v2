#include "client.h"



client::client(QWidget * parent):QWidget(parent),_port(0),_ip("114.55.249.200"),_orderfile("order.txt"),_message("")
{
    //调试代码
    // qDebug()<<"开始构建client";
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
    //待测试代码
    _orderfile.open(QIODevice::ReadWrite);
    QTextStream in(&_orderfile);
    // qDebug()<<"readAll:"<<in.readAll();
    int i = 0;
    while(!in.atEnd())
    {
        QByteArray order =  in.readLine().toUtf8();
        _orderandnum.insert(order,i);
        // qDebug()<<i;
        i++;
    }
    _orderfile.close();
    //测试代码

    // qDebug()<<"_client构建结束";
    // qDebug()<<_orderandnum;
    //测试代码
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
    _message  += _socket.readAll();
    //测试用代码
    QByteArray ms("|$/message1 awa--你好啊$|");
    _message  = ms;
    disserialize(_message,&_messages);
     //待测试代码
    for(auto& message:_messages)
    {
        auto i = _orderandnum.begin();
        qDebug()<<"2";
        while(1)//bug
        {
            //测试代码
            qDebug()<<"1";
            int ret = message.indexOf(i.key(),0);
            if(ret==-1)
            {
                if(i==_orderandnum.end()) break;
                i++;
            }
            else if(ret!= -1)
            {
                _handle(message,i.value());
                break;
            }
            else if(i==_orderandnum.end())
            {
                break;
            }
         }
    }
    //测试用代码
    qDebug()<<_messages;
}

void client::_handle(QByteArray &message,int mt)
{
    switch (mt)
    {
    case 0:
        _recv_message(message);
        break;
    case 1:
        _loginY();
        break;
    case 2:
        _loginF();

        break;
    case 3:
         _registerY();

        break;
    case 4:
         _registerF();
        break;
    default:
        break;
    }
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
    // client::showmessages();
}

void client::earseoder(QByteArray *content)
{
    int ret = content->indexOf("/message ");
    *content = content->mid(ret+strlen("/message "));
}

void client::showmessages()
{
    for(auto &e:_messages)
    {
        qDebug()<<e;
    }
}

//登入界面
void client::_register(QByteArray &content)
{
    content.insert(0,"/register ");
    sendTosever(content);
}

void client::_login(QByteArray &content) //to test//to do
{

    content.insert(0,"/login ");
    sendTosever(content);
    QMessageBox mb;
    // mb.setWindowFlags();
    QMetaObject::Connection cn =  connect(this,&client::enter_main,&mb,[&](){
        qDebug()<<"1";
        mb.close();
    });
    connect(&mb,&QObject::destroyed,&mb,[&](){
        qDebug()<<"2";
        disconnect(cn);
    });
    mb.setText("链接中......");
    mb.setWindowTitle("Loding....");
    mb.exec();

}

void client::_sendmessage(QByteArray &content)
{
    content.insert(0,"/message ");
    sendTosever(content);
}

void client::_loginY()
{
    QMessageBox mb;
    mb.setWindowTitle("登入成功");
    mb.setText("登入成功");
    mb.exec();
    emit enter_main();//进入聊天界面
}

void client::_loginF()
{
    QMessageBox mb;
    mb.setWindowTitle("登入失败");
    mb.setText("登入失败");
    mb.exec();
}

void client::_registerY()
{
    QMessageBox mb;
    mb.setWindowTitle("注册成功");
    mb.setText("注册成功");
    mb.exec();
}

void client::_registerF()
{
    QMessageBox mb;
    mb.setWindowTitle("注册失败");
    mb.setText("注册失败");
    mb.exec();
}
//主聊天界面
void client::_recv_message(QByteArray &message)
{
    earseoder(&message);
    emit got_message(message);
}
