#include "chatroom.h"
//UandPeventFilter
    UandPeventFilter::UandPeventFilter(QWidget* parent):QLineEdit(parent)
    {
        ;
    }

bool UandPeventFilter::eventFilter(QObject* obj,QEvent *event)
{
    if(event->type()==QEvent::KeyPress)
    {
        QKeyEvent *keyevent = static_cast<QKeyEvent*>(event);
        int key = keyevent->key();
        if(key==16777220)
        {
            if(obj->objectName()!="lineEdit_port")
            {
                if(obj->objectName()=="lineEdit_password")
                {
                    // qDebug()<<"发出了username信号";
                    emit to_username();
                }
                else
                {
                    // qDebug()<<"发出了password信号";
                    emit to_password();
                }
                // qDebug()<<"你按下了enter";
                return true;
            }
            return true;
        }
        else
        {
            qDebug()<<key;
            if(!((key>=48&&key<=57)||key==16777219)&&(obj->objectName()=="lineEdit_password"||obj->objectName()=="lineEdit_port"))
            {
                QMessageBox mb;
                mb.setWindowTitle("警告");
                mb.setText("请输入数字");
                mb.exec();
                return true;
            }
            if(obj->objectName()=="lineEdit_username"&&key==32)
            {
                QMessageBox mb;
                mb.setWindowTitle("警告");
                mb.setText("用户名不能有空格");
                mb.exec();
                return true;
            }
        }
    }
    return QLineEdit::eventFilter(obj,event);
}
//UandPeventFilter
//Top_widgetevent
// TopwidgeteventFilter::TopwidgeteventFilter(QWidget* parent):QWidget(parent)
// {
//     ;
// }

// bool TopwidgeteventFilter:: eventFilter(QObject *obj,QEvent *event)
// {
//     // QMouseEvent *mouse_event=static_cast<QMouseEvent*>(event);
//     // QPoint* offset;
//     // // QWidget *wobj = static_cast<QWidget*>(obj);
//     // if(event->button() == Qt::LeftButton)
//     // {

//     //     // qDebug()<<static_cast<QWidget*>(obj->parent())->objectName();
//     //     // qDebug()<<"鼠标按下了并且拖动了";
//     //     offset =new QPoint;
//     //     *offset = mouse_event->globalPosition().toPoint() - static_cast<QWidget*>(obj->parent())->pos();
//     //     qDebug()<<*offset;

//     //     return true;
//     //         // if(event->type()==QEvent::MouseButtonRelease)
//     //         // {
//     //         //     qDebug()<<"释放鼠标";
//     //         //     break;
//     //         // }
//     // }
//     // if(event->type()==QEvent::MouseMove)
//     // {
//     //     QMouseEvent *mouse_event=static_cast<QMouseEvent*>(event);
//     //     qDebug()<<1;

//     //     static_cast<QWidget*>(obj->parent())->move(mouse_event->globalPosition().toPoint()- *(offset));
//     //     delete offset;
//     //     return true;
//     // }


//     return false;

// }
//Top_widgetevent

//main_widgetevent
mainEventFilter::mainEventFilter (QWidget* parent ):QWidget(parent)
{
    ;
}

bool mainEventFilter::eventFilter(QObject* obj,QEvent *event)
{
    ;
}
