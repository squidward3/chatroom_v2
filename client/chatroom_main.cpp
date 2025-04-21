#include "chatroom_main.h"
#include "ui_chatroom_main.h"

#include <QStyleOption>



chatroom_main::chatroom_main(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::chatroom_main)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    ui->top_widget->setAttribute(Qt::WA_TranslucentBackground);
    ui->top_widget->installEventFilter(&_tf);
    ui->chat_widget->setAttribute(Qt::WA_TranslucentBackground);
    ui->textEdit_send->setAttribute(Qt::WA_TranslucentBackground);
    // setStyleSheet("background-color: white; color: black;border : 1px solid skyblue;");
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

void chatroom_main::on_close_btn_clicked()
{
    close();
}

