#ifndef QTHREADFORGRAPH_H
#define QTHREADFORGRAPH_H

#include <QThread>

#include "Graph/graph.h"
#include "Graph/3D/graph3d.h"

#include "Parameter/parameter.h"

/**For the events because a template class cannot take events*/
class TGraphEvents: public QThread
{
    Q_OBJECT

signals:
    void simulationFinished ();
    void simulationProgression(int);
};

template < typename GraphType >
class QThreadForGraph:
        public TGraphEvents
{

public:
    QThreadForGraph () {graphLoaded = false;}
    QThreadForGraph (Graph <ParamLD, GraphType> * g)
    {
        this->g = g;
        graphLoaded = true;
        QObject::connect(this, SIGNAL (started()),
                         this->g, SLOT (computeGraph ()));
        QObject::connect(this->g, SIGNAL (simulationFinished()),
                         this, SIGNAL (simulationFinished()));
        QObject::connect(this->g, SIGNAL (simulationProgression(int)),
                         this, SIGNAL (simulationProgression(int)));
    }

    ~QThreadForGraph (){/*Because no use of new for graph*/}

    void run ()
    {
        exec ();
        //g->computeGraph();
        //emit simulationFinished();
    }

    void loadGraph (Graph <ParamLD, GraphType> * g) {this->g = g;}
    void unloadGraph () {if (graphLoaded) g = NULL, graphLoaded=false;}

    virtual Graph < ParamLD, GraphType > * getGraph () = 0;

protected:
    Graph < ParamLD, GraphType > * g;
    bool graphLoaded;
};

/**Specialization**/
class QThreadFor3DGraph : public QThreadForGraph < Q3DSurface >
{

public:
    QThreadFor3DGraph (Graph < ParamLD, Q3DSurface > * g) :
        QThreadForGraph < Q3DSurface > (g) {}

    Graph < ParamLD, Q3DSurface > * getGraph () {return g;}
};

class QThreadFor2DGraph : public QThreadForGraph < QCustomPlot >
{

public:
    QThreadFor2DGraph (Graph < ParamLD, QCustomPlot > * g) :
        QThreadForGraph < QCustomPlot > (g) {}
    Graph < ParamLD, QCustomPlot > * getGraph () {return g;}
};
/**Specialization**/

#endif // QTHREADFORGRAPH_H

