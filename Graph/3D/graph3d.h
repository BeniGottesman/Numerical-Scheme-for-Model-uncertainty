#ifndef GRAPH3D_H
#define GRAPH3D_H

#include <QString>
#include "Graph/graph.h"
#include "Parameter/parameter.h"

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtWidgets/QSlider>

// == typedef Graph < ParamLD, Q3DSurface > G3DSP;
class Graph3D :
        //public QObject,
        public G3DSP
{
    Q_OBJECT
public:
    Graph3D () {}
    Graph3D (const Graph3D &);
    Graph3D (QString String_X, QString String_Y, QString String_Z,
             QString label_X, QString label_Z,
             double subdivision_X, double subdivision_Z,
             double min_X, double max_X,
             double min_Z, double max_Z,
             Strategy * s);

    ~Graph3D ();

    void updateValue(QString key, unsigned int i);

    //void updateParameter (QString key, double Value);

    double getStepX () const {return stepX;}
    double getStepZ () const {return stepZ;}

    double getInfX () const {return infX;}
    double getInfZ () const {return infZ;}

    double getSupX () const {return supX;}
    double getSupZ () const {return supZ;}

    double getSubdX () const {return subdivisionX;}
    double getSubdZ () const {return subdivisionZ;}

    QString getLabelX () const {return labelX;}
    QString getLabelZ () const {return labelZ;}

    void toggleModeNone()
    { m_graph->setSelectionMode(QAbstract3DGraph::SelectionNone); }
    void toggleModeItem()
    { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItem); }
    void toggleModeSliceRow()
    { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndRow
                                | QAbstract3DGraph::SelectionSlice); }
    void toggleModeSliceColumn()
    { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndColumn
                                | QAbstract3DGraph::SelectionSlice); }

    void setAxisXRange(float min, float max);
    void setAxisZRange(float min, float max);

    void setAxisMinSliderX(QSlider *slider) { m_axisMinSliderX = slider; }
    void setAxisMaxSliderX(QSlider *slider) { m_axisMaxSliderX = slider; }
    void setAxisMinSliderZ(QSlider *slider) { m_axisMinSliderZ = slider; }
    void setAxisMaxSliderZ(QSlider *slider) { m_axisMaxSliderZ = slider; }

    QSlider * getAxisMinSliderX () {return m_axisMinSliderX;}
    QSlider * getAxisMaxSliderX () {return m_axisMaxSliderX;}
    QSlider * getAxisMinSliderZ () {return m_axisMinSliderZ;}
    QSlider * getAxisMaxSliderZ () {return m_axisMaxSliderZ;}

    void adjustXMin(int min);
    void adjustXMax(int max);
    void adjustZMin(int min);
    void adjustZMax(int max);

    void setBlackToYellowGradient();
    void setGreenToRedGradient();

    void initSliders();

public Q_SLOTS:
    void computeGraph ();
    void changeTheme(int theme);

signals:
    void simulationProgression (int);
    void simulationFinished();

private:
    double stepX, stepZ;
    double infX, infZ;
    double supX, supZ;
    double indexX, indexZ;
    unsigned int subdivisionX, subdivisionZ;//redundant

    QString labelX, labelZ;

    QSurfaceDataProxy *m_surfaceProxy;
    QSurface3DSeries *surfaceSeries;

    QSlider *m_axisMinSliderX;
    QSlider *m_axisMaxSliderX;
    QSlider *m_axisMinSliderZ;
    QSlider *m_axisMaxSliderZ;
};

#endif // GRAPH3D_H
