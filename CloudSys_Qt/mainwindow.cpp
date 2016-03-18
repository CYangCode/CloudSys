#include "filehandler.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qfiledialog.h>
#include <qlist.h>
#include <qlistwidget.h>

#include "login.h"
#include "global.h"
#include "iconprovider.h"


#include <qmessagebox.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_readFileButton_clicked()
{
   QString path = QFileDialog::getOpenFileName(0, tr("打开文件"), ".", tr("所有文件(*)"));
   if(path.length() == 0) {
       QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
   } else {
       FileHandler handler;
       QString content = handler.readFile(path);
       ui->textEdit->setText(content);
   }
}

void MainWindow::on_writeFileButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(0, tr("另存为"), ".", tr("所有文件(*)"));
    if(path.length() == 0) {
        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    } else {
        FileHandler handler;
    }
}

void MainWindow::on_openDirectionButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(NULL, tr("选择文件夹"),"D:\\",QFileDialog::ShowDirsOnly);
    QDir *dir=new QDir(path);
    QStringList filter;
    //filter<<"*.dat";
    //dir->setNameFilters(filter);
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
    QString fileNames = "";
    QListWidget * fileInfos = new QListWidget;
    IconProvider provider;
    for (QList<QFileInfo>::iterator ite = fileInfo->begin(); ite != fileInfo->end(); ++ite){
        QString point = ".";
        fileInfos->addItem(new QListWidgetItem(provider.fileExtensionIcon(point + (*ite).completeSuffix()), (*ite).fileName()));
    }
    ui->textEdit->setText(fileNames);
}
