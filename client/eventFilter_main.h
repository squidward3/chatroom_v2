#pragma once
#include <QEvent>
#include <QMouseEvent>
#include <QWidget>

class topEventFilter:public QWidget
{
    Q_OBJECT
public:
    explicit topEventFilter(QWidget*parent = nullptr):QWidget (parent)
    {
        ;
    }
    ~topEventFilter()
    {
        ;
    }

    bool eventFilter(QObject* obj,QEvent* event) override
    {
        if(event->type()==QEvent::MouseButtonPress)
        {
            QWidget * wobj = static_cast<QWidget*>(obj);
            QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
            if(mouse_event->button()==Qt::LeftButton)
            {
                offset=mouse_event->globalPosition().toPoint() - wobj->parentWidget()->pos();
                return true;
            }
        }
        if(event->type()==QEvent::MouseMove)
        {
             QWidget * wobj = static_cast<QWidget*>(obj);
            QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
            if(mouse_event->buttons() & Qt::LeftButton)
            {
                wobj->parentWidget()->move(mouse_event->globalPosition().toPoint() - offset);
            }
            return true;
        }
        return false;
    }
private:
    QPoint offset;
};
