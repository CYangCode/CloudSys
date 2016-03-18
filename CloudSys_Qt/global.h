#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

class Global
{
public:
    Global();
    //加密密钥
    static QString DECODING_KEY;
    //用户名
    static QString USER_NAME;
    //服务器IP地址
    static const QString SERVER_IP;
    static const int FTP_SERVER_UPLOAD_PORT = 23335;
    static const int FTP_SERVER_DOWNLOAD_PORT = 23334;
    static const int SERVICE_COMMON_PORT = 23333;
};

#endif // GLOBAL_H
