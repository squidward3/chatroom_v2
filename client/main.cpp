#include "chatroom.h"
#include "chatroom_main.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    chatroom w;

    // client* client = w.get_client();
    // chatroom_main w2(client);

    // w2.show();

    w.show();
    // qDebug()<<"5";
    return a.exec();
}
