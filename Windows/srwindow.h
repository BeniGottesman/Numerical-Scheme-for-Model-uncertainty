#ifndef SRWINDOW_H
#define SRWINDOW_H

#include <QDialog>
#include "Parameter/parameter.h"
#include "Algorithm/strategy.h"

namespace Ui {
class SRWindow;
}

class SRWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SRWindow(ParamLD *pLD,
                      PayoffAlgorithm *NSMU,
                      QWidget *parent = 0);

    //Stopping region
    void ComputeStoppingRegion(ParamLD *pLD,
                          PayoffAlgorithm *po);
    double SolvingRoot (ParamLD *pLD, PayoffAlgorithm *NSMU);
    void bisection (double *x, double inf, double sup, int *itr);
    //Stopping region

    ~SRWindow();

private:
    Ui::SRWindow *ui;

    QVector <double> T, root_St;
};

#endif // SRWINDOW_H
