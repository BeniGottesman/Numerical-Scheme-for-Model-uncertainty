#ifndef QTHREADFORWINDOW_H
#define QTHREADFORWINDOW_H


#include <QThread>

class TWindowEvents: public QThread
{
    Q_OBJECT

signals:
    void test ();
};

template < typename Window >
class QThreadForWindow:
        public TWindowEvents
{
public:
    QThreadForWindow () {}
    QThreadForWindow (Window * w) {this->w = w;}

    ~QThreadForWindow () {}

    void run ()
    {
        w->show ();
        exec();
    }

private:
    Window * w;
};

#endif // QTHREADFORWINDOW_H
