#ifndef DRAWWINDOW_H
#define DRAWWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include "Algorithm/strategy.h"
#include "Algorithm/nsmualgorithm.h"
#include <Q3DSurface>
#include "Graph/3D/surfacegraph.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QScreen>

namespace Ui {
class DrawWindow;
}

class DrawWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DrawWindow(Graph3D *g3d, PayoffAlgorithm *abstractAlgo, QWidget *parent = 0);

    ~DrawWindow();

private:
    //Ui::DrawWindow *ui;
    SurfaceGraph * sg;
};

#endif // DRAWWINDOW_H
