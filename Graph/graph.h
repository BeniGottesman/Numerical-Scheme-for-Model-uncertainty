#ifndef GRAPH_H
#define GRAPH_H

#include <QThread>

#include <Q3DSurface>
#include <qcustomplot/qcustomplot.h>
#include <Pattern/Com/communicator.h>
#include <Parameter/parameter.h>
#include <Algorithm/strategy.h>

//PL = parameterList, GraphType = 2d, 3d etc
using namespace QtDataVisualization;

class GraphEvents : public QObject
{
public:
    /* explicit GraphEvents(QObject *parent = 0):
        QObject(parent) {}*/
    GraphEvents(){}

public Q_SLOTS:
    virtual void computeGraph () = 0;

signals:
    virtual void simulationProgression (int) = 0;
    virtual void simulationFinished () = 0;
};

template < typename PL, typename GraphType >
class Graph :
        public GraphEvents,
        public ICommunicator < PL, GraphType >
{
public:
    Graph (){}
    ~Graph () {delete m_graph;}

    GraphType * getGraph () {return m_graph;}
    void setStrategy (Strategy * s)
    {m_s = s;}

    virtual void updateValue(QString key, unsigned int i) = 0;

    virtual void updateParameter (QString key, double Value)
    {c->getT1()->setParameterValue (key, Value);}

protected:
    GraphType * m_graph;
    Strategy * m_s;
};

typedef Graph <ParamLD, Q3DSurface > G3DSP;
typedef Graph <ParamLD, QCustomPlot > G2DSP;
#endif // GRAPH_H
