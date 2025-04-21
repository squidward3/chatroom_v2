#ifndef RECEIVETHREAD_H
#include "chatroom.h"
#define RECEIVETHREAD_H
class receivethread:public QThread
{
    Q_OBJECT
public:
    receivethread (QObject * parenet = nullptr);
    QMutex messages_mutex;
protected:
    void run() override;
};

#endif // RECEIVETHREAD_H
