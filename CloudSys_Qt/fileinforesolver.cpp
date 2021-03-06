#include "fileinforesolver.h"
#include "iconprovider.h"
#include <QDebug>
#include "ftpclient.h"

FileInfoResolver::FileInfoResolver(QList<QString> fileInfos)
{
    this->fileInfos = fileInfos;
}

void FileInfoResolver::addIconToQListWidget(QListWidget * listWidget)
{
    IconProvider provider;
    //遍历fileInfos
    for (QList<QString>::iterator ite = fileInfos.begin(); ite != fileInfos.end(); ++ite) {
        //将文件后缀通过文件名解析出来
       QString tempFileName = *ite;
       int loc = tempFileName.lastIndexOf(".");
       QString extension = tempFileName.right(tempFileName.length() - loc);
       //将文件名通过IconProvider的对象转换成对应的图片文件
       listWidget->addItem(new QListWidgetItem(provider.fileExtensionIcon(extension), FtpClient::fromUtfToUnicode(tempFileName.toLatin1())));
    }
}

QList<QString> * FileInfoResolver::getFileInfosContains(QString content)
{
     content = content.toUpper();
     QList<QString> * qlist = new QList<QString>;
     for (QList<QString>::iterator ite = fileInfos.begin(); ite != fileInfos.end(); ++ite) {
         //要将QList中的单个文件名大写先转换成Unicode编码然后在进行比较
        if (FtpClient::fromUtfToUnicode((*ite).toUpper().toLatin1()).contains(content)) {
            qlist->push_back(*ite);
        }
     }
    return qlist;
}

