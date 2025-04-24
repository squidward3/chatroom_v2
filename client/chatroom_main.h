#ifndef CHATROOM_MAIN_H
#define CHATROOM_MAIN_H

#include <QWidget>
#include <QPainter>
#include <QTextBrowser>
#include "my_textbrowser_recv.h"
#include "eventFilter_main.h"
#include "client.h"

namespace Ui {
class chatroom_main;
}












class chatroom_main : public QWidget
{
    Q_OBJECT

public:
    explicit chatroom_main(client* client,QWidget *parent = nullptr);
    ~chatroom_main();
    void paintEvent(QPaintEvent* event) override;

private slots:
    void on_close_btn_clicked();

    void on_pushButton_clicked();

    void gotmessage(QByteArray message);

private:
    Ui::chatroom_main *ui;

private:
    //事件过滤器
    topEventFilter _tf;
public:
    QByteArray _username;
    client* _client;
};




#endif // CHATROOM_MAIN_H
