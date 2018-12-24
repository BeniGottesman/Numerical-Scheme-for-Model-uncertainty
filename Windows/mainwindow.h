#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include "Algorithm/option.h"
#include "Algorithm/strategy.h"
#include "Algorithm/nsmualgorithm.h"

#include "drawwindow.h"
#include "srwindow.h"

#include "Graph/qthreadforgraph.h"
//#include "Windows/qthreadforwindow.h"

#include "Graph/3D/graph3d.h"
#include "Pattern/Com/communicator.h"

#include <QtDataVisualization>
#include <Q3DSurface>
#include <QSurfaceDataProxy>

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
    void on_StoppingRegionButton_clicked();
    void on_surfaceComputation_clicked();

    //void on_actionAbout_clicked ();
    void on_OptionChoice_currentIndexChanged(const QString &PayoffMenu);

    void showSurfaceWindow ();

signals:
    void simulationRequested ();

private:
    Ui::MainWindow * ui;

    QThreadFor3DGraph * m_3DSimulationThread;
    //QThreadForWindow < DrawWindow > * w;
};

#endif // MAINWINDOW_H
