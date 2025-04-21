#include "chatroom.h"
#include "chatroom_main.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    chatroom w;
    // chatroom_main w2;
    // w2.show();
    w.show();
    return a.exec();
}
