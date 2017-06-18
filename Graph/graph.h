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

class GraphSignalsSlots : public QObject
{
    Q_OBJECT
public:
    explicit GraphSignalsSlots(QObject *parent = 0) :
        QObject(parent) {}

};

template < typename PL, typename GraphType >
class Graph :
        public GraphSignalsSlots,
        public ICommunicator < PL, GraphType >,
        public QThread
{
public:
    Graph (){}
    ~Graph () {delete m_graph;}

    GraphType * getGraph () {return m_graph;}
    void setStrategy (Strategy * s)
    {m_s = s;}

    virtual void computeGraph (Strategy *s) = 0;

    void run ()
    { computeGraph (m_s); }

signals:
    void simulationFinished ();

protected:
    GraphType * m_graph;
    Strategy * m_s;
};

typedef Graph <ParamLD, Q3DSurface > G3DSP;
typedef Graph <ParamLD, QCustomPlot > G2DSP;
#endif // GRAPH_H
