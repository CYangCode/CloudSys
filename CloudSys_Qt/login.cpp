#include "login.h"
#include "mainwindow.h"
#include "ui_login.h"
#include <QKeyEvent>
#include <QLineEdit>
#include <qmessagebox.h>
#include "global.h"
#include "icondisplayerwidget.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);

    //输入框禁用鼠标右键,文字提示
    setEditLine();
}

/**
*Description: 设置输入框的一些属性(禁用鼠标右键, 文字提示)
*
*@param: null
*
*@return: void
*/
void Login::setEditLine() {
    //禁用鼠标右键
    ui->pwd->setContextMenuPolicy(Qt::NoContextMenu);
    ui->key->setContextMenuPolicy(Qt::NoContextMenu);
    ui->confirmKey->setContextMenuPolicy(Qt::NoContextMenu);

    //文字提示
    ui->user->setPlaceholderText(QString("用户名"));
    ui->pwd->setPlaceholderText(QString("密码"));
    ui->key->setPlaceholderText(QString("文件加密口令"));
    ui->confirmKey->setPlaceholderText(QString("确认文件加密口令"));
}

Login::~Login()
{
    delete ui;
}

/**
*Description: 登录按钮响应事件
*
*@param: null
*
*@return: void
*/
void Login::on_okButton_clicked()
{
    QString user;
    QString pwd;
    QString key;
    QString confirmKey;

    user = ui->user->text();
    pwd = ui->pwd->text();
    key = ui->key->text();
    confirmKey = ui->confirmKey->text();

    if(key.isEmpty()){
        QMessageBox::information(this, "Information", "口令不能为空！");
        return;
    }
    if(key != confirmKey) {
        QMessageBox::information(this, "Information", "口令不一致！");
        return;
    }

    //新建TCP连接
    if(!newConnect()) {
        QMessageBox::information(this, "信息", "连接超时！请重试！");
        return;
    }

    //向服务端传输用户账户信息并验证身份

    login(user, pwd);

}

/**
*Description: 读取套接字中的信息, 信号readyRead()的槽
*
*@param: null
*
*@return: void
*/
void Login::readMsg()
{

    QByteArray qba = tcpSocket->readAll();
    QString str = QString::fromStdString(qba.toStdString());

    if("Y" == str) {
        this->close();
        Global::DECODING_KEY = ui->key->text();
        IconDisplayerWidget * widget = new IconDisplayerWidget;
        widget->show();
    } else {
        QMessageBox::information(this, "信息", "用户名或密码错误");
    }
}

/**
*Description: 向服务器发送信息验证用户身份
*
*@param: user, pwd
*
*@return: void
*/
void Login::login(QString user, QString pwd)
{
    Global::USER_NAME = user;
    QString postfix = "\r\n";
    QString end = "#END#\r\n"; //消息传递结束信号
    QString begin = "#L#\r\n";

    QString msg =user + postfix + pwd + postfix + end;

    tcpSocket->write(begin.toLatin1());
    tcpSocket->write(msg.toLatin1());

    //等待信号响应,调用槽函数
}

void disconnectedTCP() {

}

/**
*Description: 新建TCP连接
*
*@param: null
*
*@return: bool
*/
bool Login::newConnect()
{
    tcpSocket = new QTcpSocket();
    tcpSocket->abort(); //清除之前存在的连接
    tcpSocket->connectToHost(Global::SERVER_IP, Global::SERVICE_COMMON_PORT);
    if(!tcpSocket->waitForConnected(3 * 1000)) {
        return false;
    } else {
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readMsg())); //关联信号readyRead()和槽readMsg()
        return true;
    }
}
