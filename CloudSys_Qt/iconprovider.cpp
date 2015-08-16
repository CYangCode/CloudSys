#include "iconprovider.h"

#include <QFileIconProvider>
#include <QTemporaryFile>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>
#include <QMessageBox>

IconProvider::IconProvider()
{

}

QIcon IconProvider::fileExtensionIcon(const QString &extension) const
{
    QFileIconProvider icon_provider;
    static QIcon icon;
    QTemporaryFile tmp_file(QDir::tempPath() + QDir::separator() + QCoreApplication::applicationName() + "_XXXXXX" + extension);
    tmp_file.setAutoRemove(false);

    if(tmp_file.open())
    {
        QString file_name = tmp_file.fileName();
        tmp_file.write(QByteArray());
        tmp_file.close();
        icon = icon_provider.icon(QFileInfo(file_name));
     //   tmp_file.remove();
    }
    else
    {
        qCritical()<<QString("failed to write temporary file %1") .arg(tmp_file.fileName());
    }

    return icon;
}

QString IconProvider::fileExtensionType(const QString &extension) const
{
    QFileIconProvider icon_provider;
    QString type;
    QTemporaryFile tmp_file(QDir::tempPath() + QDir::separator() +
    QCoreApplication::applicationName() + "_XXXXXX" + extension);
    tmp_file.setAutoRemove(false);

    if(tmp_file.open())
    {
        QString file_name = tmp_file.fileName();
        tmp_file.write(QByteArray());
        tmp_file.close();

        type = icon_provider.type(QFileInfo(file_name));

        tmp_file.remove();
    }
    else
    {
        qCritical()<<QString("failed to write temporary file %1") .arg(tmp_file.fileName());
    }

    return type;
}

