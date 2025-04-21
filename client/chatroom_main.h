#ifndef CHATROOM_MAIN_H
#define CHATROOM_MAIN_H

#include <QWidget>
#include <QPainter>
#include "eventFilter_main.h"

namespace Ui {
class chatroom_main;
}

class chatroom_main : public QWidget
{
    Q_OBJECT

public:
    explicit chatroom_main(QWidget *parent = nullptr);
    ~chatroom_main();
    void paintEvent(QPaintEvent* event) override;

private slots:
    void on_close_btn_clicked();

private:
    Ui::chatroom_main *ui;
private:
    //事件过滤器
    topEventFilter _tf;
};

#endif // CHATROOM_MAIN_H
