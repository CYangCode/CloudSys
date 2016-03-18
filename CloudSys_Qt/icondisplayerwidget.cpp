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
    QMainWindow(parent),pFileInfoResolver(NULL), pDokanThread(NULL),
    ui(new Ui::IconDisplayerWidget)
{
    ui->setupUi(this);
    //文字提示
    ui->searchLineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    ui->searchLineEdit->setPlaceholderText(QString("请输入搜索内容"));
    pDokanThread = new DokanThread();
    //当搜索框进行输入时，自动调用searchListByName方法
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
    QList<QString> * list = pFileInfoResolver->getFileInfosContains(searchContent);
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
    //pDokanThread->start();
    refreshIconInfo();
    QMainWindow::show();
}

void IconDisplayerWidget::refreshIconInfo()
{
    TCP tcp(Global::SERVER_IP, Global::SERVICE_COMMON_PORT);
    tcp.send("#F#");//获取图标的指令
    tcp.send(Global::USER_NAME);
    tcp.send("#END#");

    QString fileNameStr = tcp.receive();
    tcp.shutdown();
    QStringList fileList = fileNameStr.split("*");//要求返回的文件信息是类似于"1.txt*2.doc*3.pdf"这样的格式
    if(fileList.length() == 1 && fileList.front() == ""){
        this->getListWidget()->clear();
        return;
    }
    if (pFileInfoResolver != NULL)
        delete pFileInfoResolver;
    pFileInfoResolver = new FileInfoResolver(fileList);
    this->getListWidget()->clear();
    pFileInfoResolver->addIconToQListWidget(this->getListWidget());
}

void IconDisplayerWidget::closeEvent(QCloseEvent * event)
{
   // pDokanThread->quit();
    QWidget::closeEvent(event);
}

void IconDisplayerWidget::on_uploadButton_clicked()
{

    QString originalPath = QFileDialog::getOpenFileName(0, tr("选择上传文件"), ".", tr("所有文件(*)"));

    if(originalPath.length() == 0) {
        QMessageBox::information(NULL, tr("提示"), tr("你没有选择任何文件!"));
        return;
    }
    //初始化虚拟磁盘
    QString cmd = "D:\\Codes\\Dokan\\dokany-0.7.3-RC4\\Win32\\Debug\\mirror /r d:/test /l m /u ";
    cmd += Global::DECODING_KEY;
    pDokanThread->setCmd(cmd);
    pDokanThread->start();
    QThread::sleep(1);
    //将文件写入虚拟磁盘
    FileHandler fileHandler;
    QString path = "M:/" + fileHandler.getFileNameByFilePath(originalPath);
    fileHandler.writeFile(path, fileHandler.readFile(originalPath));
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
    pDokanThread->quit();
    refreshIconInfo();
}

void IconDisplayerWidget::on_downLoadButton_clicked()
{
    QList<QListWidgetItem*> list = ui->iconPlayerWidget->selectedItems();
    if (list.isEmpty()){
        QMessageBox::warning(this, "警告", "你没有选中任何文件");
        return;
    }

    QString selectedFileName = list.front()->text();
  //  qDebug() << selectedFileName;
    QString path = QFileDialog::getExistingDirectory(NULL, tr("选择下载路径"),"D:\\",QFileDialog::ShowDirsOnly);
    if (path.isEmpty()) {
        return;
    }

    //TODO 将选中的文件通过socket存储到通过文件选择器获得的文件夹目录下

    TCP tcp(Global::SERVER_IP, Global::SERVICE_COMMON_PORT);
    tcp.send("#D#");
    tcp.send(Global::USER_NAME);
    tcp.send("#END#");

    FtpClient client;
    //初始化虚拟磁盘
    QString cmd = "D:\\Codes\\Dokan\\dokany-0.7.3-RC4\\Win32\\Debug\\mirror /r d:/test /l m /x ";
    cmd += Global::DECODING_KEY;
    pDokanThread->setCmd(cmd);
    pDokanThread->start();
    //先下载到虚拟磁盘中
    client.downloadFile(selectedFileName, "M:/" + selectedFileName);
    //再转入磁盘指定位置
    FileHandler handler;
    handler.writeFile(path + "/" + selectedFileName, handler.readFile("M:/" + selectedFileName));
    pDokanThread->quit();
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

void IconDisplayerWidget::terminateDokan()
{
    system("^c");
}
