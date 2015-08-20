#include "filehandler.h"

#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>

FileHandler::FileHandler()
{

}

QByteArray FileHandler::readFile(QString filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Warning", "文件打开错误");
    }
    QByteArray content = file.readAll();
    file.close();
    return content;
}

int FileHandler::writeFile(QString filePath, QString content)
{
//    QFile file(fileName);
//    if (!file.open(QIODevice::WriteOnly)) {
//        QMessageBox::warning(0, "警告", "文件下载失败");
//        return;
//    }

//    QTextStream out(&file);
//    out << content;
//    file.close();

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(0, "警告", "文件下载失败");
        return -1;
    }
    QTextStream out(&file);
    out << content;
    file.close();

    return 1;
}


int FileHandler::deleteFile(QString filePath)
{
     QFile file;
     file.remove(filePath);
     return 1;
}

QString FileHandler::getFileNameByFilePath(QString filePath)
{
    QFileInfo fileInfo(filePath);
    return fileInfo.fileName();
}

