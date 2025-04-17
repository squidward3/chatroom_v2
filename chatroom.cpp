#include "chatroom.h"
#include "ui_chatroom.h"
#include <QMessageBox>




chatroom::chatroom(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::chatroom)
    ,_regex("[48-57]")

{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    ui->lineEdit_username->installEventFilter(&_Filter);
    ui->lineEdit_password->installEventFilter(&_Filter);
    ui->lineEdit_port->installEventFilter(&_Filter);
    ui->widget_top->installEventFilter(&_topFilter);
    connect(&_Filter,&UandPeventFilter::to_password,this,[&](){
        ui->lineEdit_password->setFocus();
    });
    connect(&_Filter,&UandPeventFilter::to_username,this,[&](){
        ui->lineEdit_username->setFocus();
    });
    // connect(&_topFilter,&TopwidgeteventFilter::widget_move,this,&chatroom::move_main_widget);
}

void chatroom::paintEvent(QPaintEvent* Event)
{
    QWidget::paintEvent(Event);
    Q_UNUSED(Event);
    QPainter p (this);
    QStyleOption opt;
    opt.initFrom(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    QBrush br;
    br.setColor(QRgba64::fromRgba64(0, 0, 0, 65535));
    p.setBrush(palette().window());
    // p.setBrush(br);
    // QRect rect(10, 10, 100, 100);
    p.drawRoundedRect(opt.rect,15,15);
}

chatroom::~chatroom()
{
    delete ui;

}

// void chatroom::move_main_widget(QPoint _move)
// {
//     this->move(_move);
// }

void chatroom::on_close_clicked()
{
    close();
}

void chatroom::on_connect_btn_clicked()
{
    int i = ui->lineEdit_port->text().toInt();
    // qDebug()<<i;
    _client.setport(i);
    _client.tcp_connect();
    ; // if(ui->lineEdit_port->)
}


