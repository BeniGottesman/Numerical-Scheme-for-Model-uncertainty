#ifndef GRAPH_H
#define GRAPH_H

#include <Q3DSurface>
#include <qcustomplot/qcustomplot.h>
#include <Pattern/Com/communicator.h>
#include <Parameter/parameter.h>

//PL = parameterList, GraphType = 2d, 3d etc
using namespace QtDataVisualization;

template < typename PL, typename GraphType >
class Graph : public ICommunicator < PL, GraphType >
{
public:
    Graph (){}
    //Graph (Communicator <PL, GraphType> &c)
    //{this->c->setT2(this);}

    ~Graph () {delete graph;}

    GraphType * getGraph () {return graph;}

protected:
    GraphType * graph;
};

typedef Graph <ParamLD, Q3DSurface > G3DSP;
typedef Graph <ParamLD, QCustomPlot > G2DSP;
#endif // GRAPH_H
