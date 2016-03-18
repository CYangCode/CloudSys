#include "dokanthread.h"
#include "global.h"

DokanThread::DokanThread()
{
    cmd = "D:\\Codes\\Dokan\\dokany-0.7.3-RC4\\Win32\\Debug\\mirror /r d:/test /l m ";
}

void DokanThread::setCmd(QString cmd)
{
    this->cmd = cmd;
}

/**
 * @brief DokanThread::run 运行dokan部分
 */
void DokanThread::run()
{
    //通过mirror进行控制，进行挂载

    system(cmd.toLatin1().data());
}

void DokanThread::quit()
{
    //删除m盘的所有内容
   // system("del /Q m:\*");
    //通过dokanctl进行控制，解除挂载
    system("D:\\Codes\\Dokan\\dokany-0.7.3-RC4\\Win32\\Debug\\dokanctl /u m");
    QThread::quit();
}
