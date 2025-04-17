#ifndef DRAPWINDOWS_H
#define DRAPWINDOWS_H
#include "chatroom.h"
class drapwindows:public QWidget
{
    Q_OBJECT
public:
    drapwindows(QWidget * parent= nullptr):QWidget(parent)
    {
        ;
    }
protected:
    QPoint offset;
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            offset = event->globalPosition().toPoint() - parentWidget()->pos();
            event->accept();
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (event->buttons() & Qt::LeftButton) {
            parentWidget()->move(event->globalPosition().toPoint() - offset);
            event->accept();
        }
    }
};
#endif // DRAPWINDOWS_H
