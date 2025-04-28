#include "chatroom_main.h"
#include "ui_chatroom_main.h"

#include <QStyleOption>



chatroom_main::chatroom_main(client* client,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::chatroom_main)
    , _client(client)
{
    //测试代码
    // qDebug()<<"开始构建chatroom_main";
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    ui->top_widget->setAttribute(Qt::WA_TranslucentBackground);
    ui->top_widget->installEventFilter(&_tf);
    ui->chat_widget->setAttribute(Qt::WA_TranslucentBackground);
    ui->textEdit_send->setAttribute(Qt::WA_TranslucentBackground);
    ui->textBrowser_recv->getname(_username);
    connect(_client,&client::got_message,this,&chatroom_main::gotmessage);
    // setStyleSheet("background-color: white; color: black;border : 1px solid skyblue;");
    //测试代码



    // client->_getmessage();

    // QByteArray message ("/loginF 你爹 测试");
    // _client->_login(message);
    // QThread::sleep(3);
    // _client->_loginY();
    // _client->_handle(message,2);
    // qDebug()<<"结束构建chatroom_main";
}

chatroom_main::~chatroom_main()
{
    delete ui;
}

void chatroom_main::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPainter p (this);
    QStyleOption opt;
    opt.initFrom(this);
    QPalette pale =  opt.palette;
    pale.setColor(QPalette::Window,QColor(Qt::white));
    // setPalette(pale);
    setAutoFillBackground(true);
    p.setBrush(pale.color(QPalette::Window));
    p.drawRoundedRect(opt.rect,15,15);
}
//槽函数
void chatroom_main::on_close_btn_clicked()
{
    close();
}


void chatroom_main::on_pushButton_clicked()
{
    //测试代码
    // _username = "squidward";
    QByteArray send = ui->textEdit_send->toPlainText().toUtf8();
    ui->textEdit_send->clear();
     send.insert(0,_username+"--");
    ui->textBrowser_recv->toshow(send);

    _client->_sendmessage(send);
}

void chatroom_main::gotmessage(QByteArray message)
{
    ui->textBrowser_recv->toshowother(message);
}

