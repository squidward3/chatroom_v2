#include "chatroom.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    chatroom w;
    w.show();
    return a.exec();
}
