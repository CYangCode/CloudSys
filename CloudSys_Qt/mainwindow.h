#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    /**
     * 打开文件选择框并将所选文件内容显示在textEdit中
     * @param
     * @return
    */
    void on_readFileButton_clicked();
    /**
     * 打开文件并在textEdit中文件存储于通过文件选择框选择的文件中
     * @param
     * @return
    */
    void on_writeFileButton_clicked();
    /**
     * 通过打开文件选择框找到指定文件夹获得文件夹中所有的文件
     * @param
     * @return
    */
    void on_openDirectionButton_clicked();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
