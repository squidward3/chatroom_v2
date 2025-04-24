
// #ifndef CLIENT_H

// #define CLIENT_H
#pragma once
#include <QWidget>
#include <QPainter>
#include <QDialog>
#include <QStyleOption>
#include <QMessageBox>
#include <QString>
#include <QEvent>
#include <QKeyEvent>
#include <QLineEdit>
#include <QRgba64>
#include <QPoint>
#include <QTcpSocket>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QFile>

#define FLAG_END "$|"
#define FLAG_FRONT "|$"

// enum Orders
// {
//     MESSAGE = 0,
//     LOGINY =1,
//     LOGINF = 2,
//     REGISTERY =3,
//     REGISTERF =4
// };

class client:public QWidget
{
    Q_OBJECT
public:
    client(QWidget * parent);
    ~client();
    void setport(int port);
    void tcp_connect();
    void sendTosever(QByteArray & massage);
    void serialize(QByteArray &message);
    void disserialize(QByteArray &message,QVector<QByteArray>* messages);
    void earseoder(QByteArray *content);
    //登入界面
    void _register(QByteArray &content);
    void _login(QByteArray &content);
    void _sendmessage(QByteArray &content);
    void _loginY();
    void _loginF();
    void _registerY();
    void _registerF();
    //主聊天界面
    void _handle(QByteArray &message,int mt);
    void _recv_message(QByteArray &message);
    //测试用代码
    void showmessages();
    //槽函数//
    void _getmessage();
signals:
    void success_conected();
    void enter_main();
//主聊天窗口的信号函数
    void got_message(QByteArray message);


private:
    int _port;
    QString _ip;
    QTcpSocket _socket;
    QByteArray _message;
    // Orders _order;
private:
    QFile _orderfile;
    QVector<QByteArray> _messages;
    QMap<QByteArray,int> _orderandnum;
    QMap<QByteArray,int> _messageandorder;
};



// #endif // CLIENT_H
