#ifndef QTHREADEX_H
#define QTHREADEX_H

#include <QThread>

class QThreadEx : public QThread
{
protected:
    void run() { exec(); }

};

#endif // QTHREADEX_H
