#ifndef MY_TEXTBROWSER_RECV_H
#define MY_TEXTBROWSER_RECV_H

#include <QTextBrowser>

class my_textBrowser_recv:public QTextBrowser
{
    Q_OBJECT
public:
    my_textBrowser_recv(QWidget* parent = nullptr):QTextBrowser(parent)
    {
        ;
    }
    ~my_textBrowser_recv()
    {
        ;
    }
    void getname(QByteArray& name)
    {
        _username=name;
    }

    void toshow(QByteArray& content)
    {
        append(content);
    }

    void toshowother(QByteArray& content)
    {
        append(content);
    }
public:
    QByteArray _username;
};
#endif // MY_TEXTBROWSER_RECV_H
