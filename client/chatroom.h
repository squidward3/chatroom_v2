
// #ifndef CHATROOM_H
// #define CHATROOM_H
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
#include "chatroom_main.h"
#include "client.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class chatroom;
}
QT_END_NAMESPACE
//eventFilter
class UandPeventFilter:public QLineEdit
{
    Q_OBJECT
public:
    explicit UandPeventFilter(QWidget* parent=nullptr );
signals:
    void to_password();
    void to_username();
protected:
    bool eventFilter(QObject* obj,QEvent *event) override;
};

// class TopwidgeteventFilter:public QWidget
// {
//     Q_OBJECT
// public:
//     explicit TopwidgeteventFilter(QWidget* parent = nullptr);
// signals:
//     void widget_move(QPoint movePos);

// protected:
//     bool eventFilter(QObject *obj,QEvent *event) override;
// };

class mainEventFilter:public QWidget
{
public:
    explicit mainEventFilter (QWidget* parent = nullptr);
protected:
    bool eventFilter(QObject* obj,QEvent *event) override;
};

//eventFilter



class chatroom : public QWidget
{
    Q_OBJECT

public:
    chatroom(QWidget *parent = nullptr);
    ~chatroom();
    void paintEvent(QPaintEvent* Event) override;
    QByteArray getusenameandpassword ();

private slots:
    void on_close_clicked();
    // void move_main_widget(QPoint _move);
    void on_connect_btn_clicked();
    void on_register_btn_clicked();

    void on_login_btn_clicked();

private:


private:

    Ui::chatroom *ui;

    QRegularExpression _regex;
    UandPeventFilter _Filter;
    // TopwidgeteventFilter _topFilter;
    client _client;
    QThread _receive;
    chatroom_main * _main;
};






// #endif // CHATROOM_H
