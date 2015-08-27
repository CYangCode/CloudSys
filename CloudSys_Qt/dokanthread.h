#ifndef DOKANTHREAD_H
#define DOKANTHREAD_H

#include <QThread>

class DokanThread: public QThread
{
    Q_OBJECT
public:
    DokanThread();
    void run();
};

#endif // DOKANTHREAD_H
