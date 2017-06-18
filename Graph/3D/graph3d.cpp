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

    int xdiv = getSubdX ();
    m_axisMinSliderX->setMaximum(xdiv-1);
    m_axisMinSliderX->setValue(0);
    m_axisMaxSliderX->setMaximum(xdiv);
    m_axisMaxSliderX->setValue(xdiv);

    int zdiv = getSubdZ ();
    m_axisMinSliderZ->setMaximum(zdiv-1);
    m_axisMinSliderZ->setValue(0);
    m_axisMaxSliderZ->setMaximum(zdiv);
    m_axisMaxSliderZ->setValue(zdiv);

    //Make a copy
    surfaceProxy = new QSurfaceDataProxy ();
    surfaceSeries = new QSurface3DSeries(surfaceProxy);

    surfaceSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    surfaceSeries->setFlatShadingEnabled(true);
}

Graph3D::Graph3D(QString String_X, QString String_Y, QString String_Z,
                 QString label_X, QString label_Z,
                 double subdivision_X, double subdivision_Z,
                 double min_X, double max_X,
                 double min_Z, double max_Z)
{
    m_graph = new Q3DSurface();

    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);

    m_graph->axisX()->setTitle (String_X);
    m_graph->axisY()->setTitle (String_Y);
    m_graph->axisZ()->setTitle (String_Z);

    m_graph->axisX()->setTitleVisible (true);
    m_graph->axisY()->setTitleVisible (true);
    m_graph->axisZ()->setTitleVisible (true);

    //If we want all the points on the axes
    //graph->axisX()->setSegmentCount(subdivision_X);
    //graph->axisZ()->setSegmentCount(subdivision_Z);

    m_graph->axisX()->setLabelFormat("%.2f");
    m_graph->axisZ()->setLabelFormat("%.2f");

    subdivisionX = subdivision_X;
    subdivisionZ = subdivision_Z;

    infX = min_X;
    supX = max_X;

    infZ = min_Z;
    supZ = max_Z;

    stepX = (supX-infX)/subdivisionX;
    stepZ = (supZ-infZ)/subdivisionZ;

    m_graph->axisX()->setRange(infX, supX);
    m_graph->axisY()->setAutoAdjustRange(true);
    m_graph->axisZ()->setRange(infZ, supZ);

    m_graph->axisX()->setLabelAutoRotation(30);
    m_graph->axisY()->setLabelAutoRotation(90);
    m_graph->axisZ()->setLabelAutoRotation(30);

    labelX = label_X;
    labelZ = label_Z;

    surfaceProxy = new QSurfaceDataProxy ();
    surfaceSeries = new QSurface3DSeries(surfaceProxy);

    surfaceSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    surfaceSeries->setFlatShadingEnabled(true);
}

void Graph3D::initSliders()
{
    int xdiv = getSubdX ();
    m_axisMinSliderX->setMaximum(xdiv-1);
    m_axisMinSliderX->setValue(0);
    m_axisMaxSliderX->setMaximum(xdiv);
    m_axisMaxSliderX->setValue(xdiv);

    int zdiv = getSubdZ ();
    m_axisMinSliderZ->setMaximum(zdiv-1);
    m_axisMinSliderZ->setValue(0);
    m_axisMaxSliderZ->setMaximum(zdiv);
    m_axisMaxSliderZ->setValue(zdiv);
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

void Graph3D::computeGraph (Strategy *s)
{
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;

    double y0 = 0;

    double x0 = infX;
    double x0_inf = infX;
    double x0_sup = supX;
    double x0_subdiv = subdivisionX;

    double z0 = infZ;
    double z0_inf = infZ;
    double z0_sup = supZ;
    double z0_subdiv = subdivisionZ;

    dataArray->reserve(z0_subdiv /*+ 1.0f*/);//verify this line
    for (int i = 1; z0 <= z0_sup; i++)
    {
        s->getStrategy()->getPayoff()->setMaturity(z0);
        QSurfaceDataRow *newRow = new QSurfaceDataRow(x0_subdiv + 1.0f);
        // Keep values within range bounds,
        // since just adding step can cause minor drift due
        // to the rounding errors.
        int index = 0;
        for(int j = 1; x0 <= x0_sup; j++)
        {
            y0 = s->execute(*getCommunicator()->getT1());//V00
            (*newRow)[index++].setPosition(QVector3D(x0, y0, z0));
            //std::cout << "T = " << z0 << ", S0 = " << x0 << ", V00 = " << y0 <<"\n";
            updateValue("x", j);
            x0 = x0_inf + j*getStepX();
            if (getStepX() == 0) //If X is reduced to a singleton
                break;
        }
        *dataArray << newRow;

        updateValue("x", 0);//we reset the x parameter
        x0 = getInfX ();

        updateValue("z", i);
        z0 = z0_inf + i*getStepZ();
        if (getStepZ() == 0)//Z is reduced to a singleton
            break;
    }

    surfaceProxy->resetArray(dataArray);

    updateValue("z", 0);//we reset the z parameter

    m_graph->addSeries (surfaceSeries);
}

void Graph3D::setAxisXRange(float min, float max)
{
    m_graph->axisX()->setRange(min, max);
}

void Graph3D::setAxisZRange(float min, float max)
{
    m_graph->axisZ()->setRange(min, max);
}

void Graph3D::setBlackToYellowGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::black);
    gr.setColorAt(0.33, Qt::blue);
    gr.setColorAt(0.67, Qt::red);
    gr.setColorAt(1.0, Qt::yellow);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void Graph3D::setGreenToRedGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::darkGreen);
    gr.setColorAt(0.5, Qt::yellow);
    gr.setColorAt(0.8, Qt::red);
    gr.setColorAt(1.0, Qt::darkRed);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}


void Graph3D::adjustXMin(int min)
{
    double m_stepX = getStepX();
    double m_rangeMinX = getInfX();
    float minX = m_stepX * float(min) + m_rangeMinX;

    int max = m_axisMaxSliderX->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderX->setValue(max);
    }
    float maxX = m_stepX * max + m_rangeMinX;

    setAxisXRange(minX, maxX);
}

void Graph3D::adjustXMax(int max)
{
    double m_stepX = getStepX();
    double m_rangeMinX = getInfX();
    float maxX = m_stepX * float(max) + m_rangeMinX;

    int min = m_axisMinSliderX->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderX->setValue(min);
    }
    float minX = m_stepX * min + m_rangeMinX;

    setAxisXRange(minX, maxX);
}

void Graph3D::adjustZMin(int min)
{
    double m_stepZ = getStepZ();
    double m_rangeMinZ = getInfZ();
    float minZ = m_stepZ * float(min) + m_rangeMinZ;

    int max = m_axisMaxSliderZ->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderZ->setValue(max);
    }
    float maxZ = m_stepZ * max + m_rangeMinZ;

    setAxisZRange(minZ, maxZ);
}

void Graph3D::adjustZMax(int max)
{
    double m_stepZ = getStepZ();
    double m_rangeMinZ = getInfZ();
    float maxX = m_stepZ * float(max) + m_rangeMinZ;

    int min = m_axisMinSliderZ->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderZ->setValue(min);
    }
    float minX = m_stepZ * min + m_rangeMinZ;

    setAxisZRange(minX, maxX);
}

void Graph3D::changeTheme(int theme)
{
    m_graph->activeTheme()->setType(Q3DTheme::Theme(theme));
}

Graph3D::~Graph3D ()
{
    delete surfaceProxy;
    delete surfaceSeries;
}
