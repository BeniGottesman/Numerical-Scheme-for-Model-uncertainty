#include "graph3d.h"

Graph3D::Graph3D (Graph3D & g)//revoir le constructeur copie
{
    infX = g.getInfX();
    supX = g.getSupX();
    stepX = g.getStepX();
    subdivisionX = g.getSubdX();
    labelX = g.getLabelX();

    infZ = g.getInfZ();
    supZ = g.getSupZ();
    stepZ = g.getStepZ();
    subdivisionZ = g.getSubdZ();
    labelZ = g.getLabelZ();
}

Graph3D::Graph3D(QString String_X, QString String_Y, QString String_Z,
                 QString label_X, QString label_Z,
                 double subdivision_X, double subdivision_Z,
                 double min_X, double max_X,
                 double min_Z, double max_Z)
{
    graph = new Q3DSurface();

    graph->setAxisX(new QValue3DAxis);
    graph->setAxisY(new QValue3DAxis);
    graph->setAxisZ(new QValue3DAxis);

    graph->axisX()->setTitle (String_X);
    graph->axisY()->setTitle (String_Y);
    graph->axisZ()->setTitle (String_Z);

    graph->axisX()->setTitleVisible (true);
    graph->axisY()->setTitleVisible (true);
    graph->axisZ()->setTitleVisible (true);

    //graph->axisX()->setSegmentCount(subdivision_X);
    //graph->axisZ()->setSegmentCount(subdivision_Z);

    graph->axisX()->setLabelFormat("%.2f");
    graph->axisZ()->setLabelFormat("%.2f");

    subdivisionX = subdivision_X;
    subdivisionZ = subdivision_Z;

    infX = min_X;
    supX = max_X;

    infZ = min_Z;
    supZ = max_Z;

    stepX = (supX-infX)/subdivisionX;
    stepZ = (supZ-infZ)/subdivisionZ;

    graph->axisX()->setRange(infX, supX);
    graph->axisY()->setAutoAdjustRange(true);
    graph->axisZ()->setRange(infZ, supZ);
    //graph->axisZ()->setAutoAdjustRange(true);

    graph->axisX()->setLabelAutoRotation(30);
    graph->axisY()->setLabelAutoRotation(90);
    graph->axisZ()->setLabelAutoRotation(30);

    labelX = label_X;
    labelZ = label_Z;
}

void Graph3D::updateValue(QString key, unsigned int i)
{
    double newValue;

    double inf = infZ;
    double sup = supZ;
    double step = stepZ;
    QString label = labelZ;

    if (key == "x")
        inf = infX, sup = supX, step = stepX, label = labelX;

    newValue = inf + i*step;
    if (newValue > sup)
        updateParameter (label, sup);
    else
        updateParameter (label, newValue);
}

//inline
void Graph3D::updateParameter (QString key, double Value)
{
    c->getT1()->setParameterValue (key, Value);
}
