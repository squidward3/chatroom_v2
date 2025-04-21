
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

#define FLAG_END "$|"
#define FLAG_FRONT "|$"


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

    //登入界面
    void _register(QByteArray &content);
    void _login(QByteArray &content);
    //测试用代码
    void showmessages();
    //槽函数//
    void _getmessage();
signals:
    void success_conected();
    void enter_main();


private:
    int _port;
    QString _ip;
    QTcpSocket _socket;
    QByteArray _message;
private:

    QVector<QByteArray> _messages;
};



// #endif // CLIENT_H
