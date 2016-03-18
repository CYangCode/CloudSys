#include "filehandler.h"
#include "ftpclient.h"
#include "global.h"

#include <QIoDevice>
#include <QMessageBox>
#include <QTextStream>

const QString FtpClient::DEFAULT_PWD = "0";

FtpClient::FtpClient(QObject *parent) :
    QObject(parent)
{
  //该QUrl类提供了一个方便的接口,用于处理URL
    url.setScheme("ftp");//设置该计划描述了URL的类型（或协议）
    url.setHost(Global::SERVER_IP);//设置主机地址

  //  url.setPath(fromUnicodeToUtf(FILE_NAME.toLatin1()));//设置URL路径。该路径是自带权限后的URL的一部分，但在查询字符串之前
    url.setPort(Global::FTP_SERVER_UPLOAD_PORT);//设置URL的端口。该端口是URL的权限的一部分，如setAuthority（描述）。端口必须是介于0和65535（含）。端口设置为-1表示该端口是不确定的。
    url.setPassword(DEFAULT_PWD); //设置ftp用户密码, 使用默认密码
    url.setUserName(Global::USER_NAME);//设置tfp用户名
    request = new QNetworkRequest;
    request->setUrl(url); //这只request的请求
    manager = new QNetworkAccessManager;//QNetworkAccessManager 允许发送网络请求和接收回复

}

void FtpClient::downloadFile(QString downloadFileName, QString desFilePath)
{
    url.setPort(Global::FTP_SERVER_DOWNLOAD_PORT);
    url.setPath(fromUnicodeToUtf(downloadFileName));
//    QMessageBox::information(0, "url" , url.toString());
 //   qDebug() << "url" + url.toString();
    request->setUrl(url);

    reply = manager->get(*request);
    QEventLoop * loop  = new QEventLoop;
    connect(manager, SIGNAL(finished(QNetworkReply*)), loop, SLOT(quit()));
    loop->exec();

    QByteArray byte = reply->readAll();
   // qDebug() << content.length();
    FileHandler fileHandler;
    fileHandler.writeFile(desFilePath, byte);
   // reply->deleteLater();   //最后要释放reply对象
}

void FtpClient::uploadFile(QString filePath)
{
    FileHandler fileHandler;
    QString fileName = fileHandler.getFileNameByFilePath(filePath);
    QByteArray content = fileHandler.readFile(filePath);

    url.setPath(fromUnicodeToUtf(fileName));
    url.setPort(Global::FTP_SERVER_UPLOAD_PORT);
//"FtpClient::uploadFile::url:" +
 //   qDebug() <<  url.toString();
    QNetworkRequest * request = new QNetworkRequest(url);
    request->setUrl(url);
    manager->put(*request, content);
    QEventLoop * loop = new QEventLoop;
    connect(manager, SIGNAL(finished(QNetworkReply*)), loop, SLOT(quit()));
    loop->exec();
//    qDebug() << "FtpClient::uploadFile loop end";
    delete request;
}

QString FtpClient::fromUtfToUnicode(QByteArray str)//传进来的是%E6%B5%8B%E8%AF%952.txt这样的字符串
{
   // qDebug() << "FtpClient::fromUtfToUnicode::str:" + str;
    QTextCodec * codec = QTextCodec::codecForName("utf-8");
    QByteArray byte = QByteArray::fromPercentEncoding(str);
   // qDebug() << "FtpClient::fromUtfToUnicode::byte:" + byte;
    return codec->toUnicode(byte);
}

QString FtpClient::fromUnicodeToUtf(QString &str)
{
    return str.toUtf8().toPercentEncoding();
}

QByteArray FtpClient::fromUnicodeToUtf(QByteArray str)//传进来的是"测试.txt"这样的字符串,假设对应的是"/u4234/u234"
{
    QTextCodec * codec = QTextCodec::codecForName("utf-8");
    QByteArray byte = codec->fromUnicode(str);
    return byte.toPercentEncoding();
}
