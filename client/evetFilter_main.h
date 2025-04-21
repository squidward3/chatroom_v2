#ifndef EVETFILTER_MAIN_H
#define EVETFILTER_MAIN_H

#endif // EVETFILTER_MAIN_H
#include "chatroom_main.h"

#include <QEvent>
#include <QMouseEvent>

class top_widgetFilter:public QWidget
{
    Q_OBJECT
    top_widgetFilter(QWidget* parent = nullptr):QWidget(parent)
    {
        ;
    }
    ~top_widgetFilter()
    {
        ;
    }
    bool eventFilter(QObject * obj, QEvent* event)
    {
        if(event->type()==QEvent::MouseButtonPress)
        {
            QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);

        }
    }
};
