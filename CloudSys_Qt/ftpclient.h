#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QMainWindow>
#include <qstring.h>
#include <qfile.h>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "qdebug.h"
#include <QtGui>
#include <QUrl>

class FtpClient : public QObject
{
    Q_OBJECT
public:
    explicit FtpClient(QObject *parent = 0);
    /**
     * 从ftp获取信息并将信息存储于指定文件中
     * @param  filename 文件内容
     * @return
    */
    void downloadFile(QString downloadFileName, QString desFilePath);
    /**
     * 发送信息
     * @param data 信息内容
     * @return
    */
    void uploadFile(QString filePath);
    /**
     * 将utf的信息转为Unicode编码的信息
     * @param str utf编码的信息
     * @return Unicode编码的信息
    */
    static QString fromUtfToUnicode(QByteArray str);
    /**
     * 将Unicode的信息转为Utf-8编码的信息
     * @param str unicode编码的信息
     * @return Utf-8编码的信息
    */
    static QString fromUnicodeToUtf(QString & str);
    /**
     * 将Unicode的信息转为Utf-8编码的信息
     * @param str unicode编码的信息
     * @return Utf-8编码的信息
    */
    static QByteArray fromUnicodeToUtf(QByteArray str);

private slots:

private:
    QNetworkAccessManager *manager;
    QNetworkReply * reply;
    QString content;
    QUrl url;
    QNetworkRequest * request;//该QNetworkReply类包含的数据和标题,对QNetworkAccessManager发送请求
    static const QString DEFAULT_PWD;
    static const int FTP_SERVER_UPLOAD_PORT = 23334;
    static const int FTP_SERVER_DOWNLOAD_PORT = 23335;
};

#endif // FTPCLIENT_H
