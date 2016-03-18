#ifndef ICONDISPLAYERWIDGET_H
#define ICONDISPLAYERWIDGET_H

#include <QMainWindow>
#include <QListWidget>
#include <QIcon>
#include "dokanthread.h"
#include "fileinforesolver.h"
namespace Ui {
class IconDisplayerWidget;
}

class IconDisplayerWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit IconDisplayerWidget(QWidget *parent = 0);
    ~IconDisplayerWidget();
    /**
     * 添加图标和图标对应的文件名
     * @param icon 文件图标
     * @param desc 图标对应的文件名
     * @return
    */
    void addIcon(QIcon icon, QString desc);
    /**
     * 获得ui中的ListWidget
     * @param
     * @return
    */
    QListWidget * getListWidget ();
    /**
     * 重载show方法,在show的同时通过socket通信将图标信息获取到,然后通过fileResolver进行解析
     * @param
     * @return
    */
    void show();
    /**
     * 通过socket获取图标信息,然后对listwidget进行更新
     * @param
     * @return
    */
    void refreshIconInfo();
protected:
    void closeEvent(QCloseEvent *);
private slots:
    void on_uploadButton_clicked();

    void searchListByName();

    void on_downLoadButton_clicked();

    void on_deleteButton_clicked();

    void terminateDokan();
private:
    Ui::IconDisplayerWidget *ui;
    FileInfoResolver * pFileInfoResolver;
    DokanThread * pDokanThread;
};

#endif // ICONDISPLAYERWIDGET_H
