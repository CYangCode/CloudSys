#include "filehandler.h"
#include "ftpclient.h"
#include "icondisplayerwidget.h"
#include "ui_icondisplayerwidget.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include "global.h"
#include "tcp.h"

IconDisplayerWidget::IconDisplayerWidget(QWidget *parent) :
    QMainWindow(parent),fileInfoResolver(NULL),
    ui(new Ui::IconDisplayerWidget)
{
    ui->setupUi(this);
    //文字提示
    ui->searchLineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    ui->searchLineEdit->setPlaceholderText(QString("请输入搜索内容"));
    connect(ui->searchLineEdit, &QLineEdit::textEdited, this, &IconDisplayerWidget::searchListByName);
}

IconDisplayerWidget::~IconDisplayerWidget()
{
    delete ui;
}

void IconDisplayerWidget::addIcon(QIcon icon, QString desc)
{
    ui->iconPlayerWidget->addItem(new QListWidgetItem(icon, desc));
}

QListWidget * IconDisplayerWidget::getListWidget()
{
    return ui->iconPlayerWidget;
}


void IconDisplayerWidget::searchListByName()
{
    QString searchContent = ui->searchLineEdit->text();
    QList<QString> * list = fileInfoResolver->getFileInfosContains(searchContent);
//    for (QList<QString>::iterator ite = list->begin(); ite != list->end(); ++ite) {
//        qDebug() << (*ite);
//    }
    ui->iconPlayerWidget->clear();
    FileInfoResolver * resolver = new FileInfoResolver(*list);
    resolver->addIconToQListWidget(this->getListWidget());
    delete(resolver);
}

void IconDisplayerWidget::show()
{
    /*
     * 获得通过服务器传过来的图标信息,并在ListWidget进行添加图标信息
     */
    // 测试--------------------------
//    QList<QString> fileInfoList;
//    fileInfoList.push_back("2.txt");
//    fileInfoList.push_back("2.doc");
//    fileInfoList.push_back("2.jpg");
//    fileInfoList.push_back("2.zip");
//    fileInfoList.push_back("2.ppt");
//    //==================================
//    fileInfoResolver = new FileInfoResolver(fileInfoList);
//    fileInfoResolver->addIconToQListWidget(this->getListWidget());
    refreshIconInfo();
    QMainWindow::show();
}

void IconDisplayerWidget::refreshIconInfo()
{
    TCP tcp(Global::SERVER_IP, Global::SERVICE_COMMON_PORT);
    tcp.send("#F#");//获取图标的指令
    tcp.send(Global::USER_NAME);
    tcp.send("#END#");

//    qDebug() << "IconDisplayerWidget::refreshIconInfo::USER_NAME:" + Global::USER_NAME;
    QString fileNameStr = tcp.receive();
    qDebug() << fileNameStr;
    tcp.shutdown();
    QStringList fileList = fileNameStr.split("*");//要求返回的文件信息是类似于"1.txt*2.doc*3.pdf"这样的格式
    if(fileList.length() == 1 && fileList.front() == ""){
        this->getListWidget()->clear();
        return;
    }
    if (fileInfoResolver != NULL)
        delete fileInfoResolver;
    fileInfoResolver = new FileInfoResolver(fileList);
    this->getListWidget()->clear();
    fileInfoResolver->addIconToQListWidget(this->getListWidget());
}

void IconDisplayerWidget::on_backButton_clicked()
{
    ui->iconPlayerWidget->clear();
    fileInfoResolver->addIconToQListWidget(this->getListWidget());
}

void IconDisplayerWidget::on_uploadButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(0, tr("选择上传文件"), ".", tr("所有文件(*)"));
    if(path.length() == 0) {
        QMessageBox::information(NULL, tr("提示"), tr("你没有选择任何文件!"));
    } else {
        qDebug() << path;
        //将文件通过ftp传至服务器端然后刷新QListWidget
        TCP tcp(Global::SERVER_IP, Global::SERVICE_COMMON_PORT);
        tcp.send("#U#");
        tcp.send(Global::USER_NAME);
        tcp.send("#END#");
        FtpClient client;
        client.uploadFile(path);
        tcp.send("#OK#");
        tcp.send("#END#");
        tcp.shutdown();
        refreshIconInfo();
    }
}

void IconDisplayerWidget::on_downLoadButton_clicked()
{
    QList<QListWidgetItem*> list = ui->iconPlayerWidget->selectedItems();
    if (list.isEmpty()){
        QMessageBox::warning(this, "警告", "你没有选中任何文件");
        return;
    }
    QString selectedFileName = list.front()->text();
    qDebug() << selectedFileName;
    QString path = QFileDialog::getExistingDirectory(NULL, tr("选择下载路径"),"D:\\",QFileDialog::ShowDirsOnly);
    //TODO 将选中的文件通过socket存储到通过文件选择器获得的文件夹目录下

    TCP tcp(Global::SERVER_IP, Global::SERVICE_COMMON_PORT);
    tcp.send("#D#");
    tcp.send(Global::USER_NAME);
    tcp.send("#END#");

    FtpClient client;
    client.downloadFile(path + "/" + selectedFileName);
    tcp.send("#OK#");
    tcp.send("#END#");
    tcp.shutdown();
}

void IconDisplayerWidget::on_deleteButton_clicked()
{
    QList<QListWidgetItem*> list = ui->iconPlayerWidget->selectedItems();
    if (list.isEmpty()){
        QMessageBox::warning(this, "警告", "你没有选中任何文件");
        return;
    }
    int confirm = QMessageBox::warning(this, "提示", "确认删除?", QMessageBox::Yes, QMessageBox::Cancel);
    if (confirm == -1) {
        return;
    }
    QString fileName = list.front()->text();
    TCP tcp(Global::SERVER_IP, Global::SERVICE_COMMON_PORT);
    tcp.send("#E#");
    tcp.send(Global::USER_NAME);
    tcp.send(FtpClient::fromUnicodeToUtf(fileName));
    tcp.send("#END#");
    QString aff = tcp.receive();
    if (aff == "Y") {
      refreshIconInfo();
    } else {
        QMessageBox::information(this, "信息", "删除失败");
    }
    tcp.shutdown();
}
