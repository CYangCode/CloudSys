#ifndef DOKANTHREAD_H
#define DOKANTHREAD_H

#include <QThread>

class DokanThread: public QThread
{
    Q_OBJECT
public:
    DokanThread();
    void setCmd(QString cmd);
    void run();
public:
    void quit();
private:
    QString cmd;
};

#endif // DOKANTHREAD_H
