#include "chatroom.h"
#include "ui_chatroom.h"
#include <QMessageBox>




chatroom::chatroom(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::chatroom)
    ,_regex("[48-57]")
    ,_client(this)
    ,_main(new chatroom_main)

{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    ui->lineEdit_username->installEventFilter(&_Filter);
    ui->lineEdit_password->installEventFilter(&_Filter);
    ui->lineEdit_port->installEventFilter(&_Filter);
    // ui->widget_top->installEventFilter(&_topFilter);
    connect(&_Filter,&UandPeventFilter::to_password,this,[&](){
        ui->lineEdit_password->setFocus();
    });
    connect(&_Filter,&UandPeventFilter::to_username,this,[&](){
        ui->lineEdit_username->setFocus();
    });
    // connect(&_topFilter,&TopwidgeteventFilter::widget_move,this,&chatroom::move_main_widget);
    ui->register_btn->setEnabled(false);

    ui->login_btn->setEnabled(false);
    connect(&_client,&client::success_conected,this,[&](){
        ui->register_btn->setEnabled(true);
        ui->login_btn->setEnabled(true);
    });
    connect(&_client,&client::enter_main,this,[&](){
        close();
        _main->show();
    });
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
    delete _main;

}

// void chatroom::move_main_widget(QPoint _move)
// {
//     this->move(_move);
// }
//槽函数
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
    QByteArray message ("/connect ");
    _client.sendTosever(message);
    // if(ui->lineEdit_port->)
}
//槽函数end

//功能性函数

QByteArray chatroom::getusenameandpassword ()
{
    QByteArray content = ui->lineEdit_username->text().toUtf8();
    content += " " + ui->lineEdit_password->text().toUtf8();

    return content;
}


void chatroom::on_register_btn_clicked()
{
    QByteArray content = getusenameandpassword();
    content.insert(0,"/register ");
    _client._register(content);
}


void chatroom::on_login_btn_clicked()
{
    QByteArray content = getusenameandpassword();
    content.insert(0,"/login ");
    _client._login(content);

}

