#ifndef SURFACEGRAPH_H
#define SURFACEGRAPH_H

#include "Algorithm/strategy.h"
#include <Parameter/parameter.h>
#include <Graph/3D/graph3d.h>

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtWidgets/QSlider>

class SurfaceGraph : public QObject
{
    Q_OBJECT

public:
    SurfaceGraph(){}
    SurfaceGraph(Strategy *s, ParamLD *pLD, Graph3D *g);
    SurfaceGraph (SurfaceGraph &sg);

    ~SurfaceGraph();

    void computeGraph ();

    void toggleModeNone() { graph->getGraph()->setSelectionMode(QAbstract3DGraph::SelectionNone); }
    void toggleModeItem() { graph->getGraph()->setSelectionMode(QAbstract3DGraph::SelectionItem); }
    void toggleModeSliceRow()
    { graph->getGraph()->setSelectionMode(QAbstract3DGraph::SelectionItemAndRow
                                                          | QAbstract3DGraph::SelectionSlice); }
    void toggleModeSliceColumn()
    { graph->getGraph()->setSelectionMode(QAbstract3DGraph::SelectionItemAndColumn
                                                             | QAbstract3DGraph::SelectionSlice); }

    void setAxisXRange(float min, float max);
    void setAxisZRange(float min, float max);

    void adjustXMin(int min);
    void adjustXMax(int max);
    void adjustZMin(int min);
    void adjustZMax(int max);

    void setBlackToYellowGradient();
    void setGreenToRedGradient();

    void initSliders ();

    void setAxisMinSliderX(QSlider *slider) { m_axisMinSliderX = slider; }
    void setAxisMaxSliderX(QSlider *slider) { m_axisMaxSliderX = slider; }
    void setAxisMinSliderZ(QSlider *slider) { m_axisMinSliderZ = slider; }
    void setAxisMaxSliderZ(QSlider *slider) { m_axisMaxSliderZ = slider; }

    QSlider * getAxisMinSliderX () {return m_axisMinSliderX;}
    QSlider * getAxisMaxSliderX () {return m_axisMaxSliderX;}
    QSlider * getAxisMinSliderZ () {return m_axisMinSliderZ;}
    QSlider * getAxisMaxSliderZ () {return m_axisMaxSliderZ;}

public Q_SLOTS:
    void changeTheme(int theme);

private:
    Strategy *s;

    Graph3D *graph;
    QSurfaceDataProxy *surfaceProxy;
    QSurface3DSeries *surfaceSeries;

    QSlider *m_axisMinSliderX;
    QSlider *m_axisMaxSliderX;
    QSlider *m_axisMinSliderZ;
    QSlider *m_axisMaxSliderZ;
};

#endif // SURFACEGRAPH_H
