#include "tcp.h"
#include <QDebug>
#include <QEventLoop>

/**
*Description: 向服务端发送消息
*
*@param: msg
*
*@return: bool
*/
bool TCP::send(QString msg)
{
    msg += "\r\n";
    tcp->write(msg.toLatin1());
    return true;
}

/**
*Description: 从套接字取回数据
*
*@param: null
*
*@return: void
*/
QString TCP::receive()
{
    int timeout = 5 * 1000; //超时时间设置
    QTimer t;
    QEventLoop q;
    t.setSingleShot(true);
    connect(&t, SIGNAL(timeout()), &q, SLOT(quit()));  //异步调用超时退出
    connect(tcp, SIGNAL(readyRead()), &q, SLOT(quit()));  //异步调用完成退出

    t.start(timeout);
    q.exec();   //loop开始,阻塞
    QByteArray qba = tcp->readAll();
    return QString::fromStdString(qba.toStdString());

}

/**
*Description: 断开连接
*
*@param:
*
*@return: voiv
*/
void TCP::shutdown()
{
    tcp->disconnectFromHost();
}

/**
*Description: 建立TCP链接.
*
*@param: address, port
*
*@return:
*/
TCP::TCP(QString address, qint16 port)
{
    tcp = new QTcpSocket();
    tcp->abort();
    tcp->connectToHost(address, port);

    if(!tcp->waitForConnected(3 * 1000)) {
       // qDebug() << "TCP初始化连接超时. TCP::TCP(QString, qint16)";
    }
}
