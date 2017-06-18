#include "surfacegraph.h"

using namespace QtDataVisualization;

SurfaceGraph::SurfaceGraph(Strategy *s,
                           ParamLD *pLD,
                           Graph3D *g)
{
    this->graph = g;
    this->s = s;//new Strategy (*s);

    this->surfaceProxy = new QSurfaceDataProxy ();
    this->surfaceSeries = new QSurface3DSeries(surfaceProxy);

    this->surfaceSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    this->surfaceSeries->setFlatShadingEnabled(true);

    this->computeGraph ();

    this->graph->getGraph()->addSeries(this->surfaceSeries);
}

void SurfaceGraph::initSliders()
{
    int xdiv = graph->getSubdX ();
    m_axisMinSliderX->setMaximum(xdiv-1);
    m_axisMinSliderX->setValue(0);
    m_axisMaxSliderX->setMaximum(xdiv);
    m_axisMaxSliderX->setValue(xdiv);

    int zdiv = graph->getSubdZ ();
    m_axisMinSliderZ->setMaximum(zdiv-1);
    m_axisMinSliderZ->setValue(0);
    m_axisMaxSliderZ->setMaximum(zdiv);
    m_axisMaxSliderZ->setValue(zdiv);
}

SurfaceGraph::SurfaceGraph(SurfaceGraph &sg)
{
    this->s = sg.s;
}

SurfaceGraph::~SurfaceGraph()
{
    delete graph;
    delete s;
}

void SurfaceGraph::computeGraph ()
{
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;

    double y0 = 0;
    double maxYValue = 0;

    double x0 = graph->getInfX();
    double x0_inf = graph->getInfX();
    double x0_sup = graph->getSupX ();
    double x0_subdiv = graph->getSubdX ();

    double z0 = graph->getInfZ();
    double z0_inf = graph->getInfZ();
    double z0_sup = graph->getSupZ ();
    double z0_subdiv = graph->getSubdZ ();

    //Make a loop
    dataArray->reserve(z0_subdiv /*+ 1.0f*/);//verify this line
    for (int i = 1; z0 <= z0_sup; i++)
    {
        this->s->getStrategy()->getPayoff()->setMaturity(z0);
        QSurfaceDataRow *newRow = new QSurfaceDataRow(x0_subdiv + 1.0f);
        // Keep values within range bounds,
        // since just adding step can cause minor drift due
        // to the rounding errors.
        int index = 0;
        for(int j = 1; x0 <= x0_sup; j++)
        {
            y0 = this->s->execute(*graph->getCommunicator()->getT1());//V00
            (*newRow)[index++].setPosition(QVector3D(x0, y0, z0));
            if (y0 > maxYValue)
                maxYValue = y0;
            std::cout << "T = " << z0 << ", S0 = " << x0 << ", V00 = " << y0 <<"\n";
            graph->updateValue("x", j);
            x0 = x0_inf + j*graph->getStepX();
            if (graph->getStepX() == 0)//X is reduced to a singleton
                break;
        }
        *dataArray << newRow;

        graph->updateValue("x", 0);//we reset the x parameter
        x0 = graph->getInfX ();

        graph->updateValue("z", i);
        z0 = z0_inf + i*graph->getStepZ();
        if (graph->getStepZ() == 0)//Z is reduced to a singleton
            break;
    }
    //graph->getGraph()->axisY()->setRange(0.0f, maxYValue + 2.0f);
    surfaceProxy->resetArray(dataArray);

    graph->updateValue("z", 0);//we reset the z parameter
}

void SurfaceGraph::setAxisXRange(float min, float max)
{
    graph->getGraph()->axisX()->setRange(min, max);
}

void SurfaceGraph::setAxisZRange(float min, float max)
{
    graph->getGraph()->axisZ()->setRange(min, max);
}

void SurfaceGraph::changeTheme(int theme)
{
    graph->getGraph()->activeTheme()->setType(Q3DTheme::Theme(theme));
}

void SurfaceGraph::setBlackToYellowGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::black);
    gr.setColorAt(0.33, Qt::blue);
    gr.setColorAt(0.67, Qt::red);
    gr.setColorAt(1.0, Qt::yellow);

    graph->getGraph()->seriesList().at(0)->setBaseGradient(gr);
    graph->getGraph()->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void SurfaceGraph::setGreenToRedGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::darkGreen);
    gr.setColorAt(0.5, Qt::yellow);
    gr.setColorAt(0.8, Qt::red);
    gr.setColorAt(1.0, Qt::darkRed);

    graph->getGraph()->seriesList().at(0)->setBaseGradient(gr);
    graph->getGraph()->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}


void SurfaceGraph::adjustXMin(int min)
{
    double m_stepX = graph->getStepX();
    double m_rangeMinX = graph->getInfX();
    float minX = m_stepX * float(min) + m_rangeMinX;

    int max = m_axisMaxSliderX->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderX->setValue(max);
    }
    float maxX = m_stepX * max + m_rangeMinX;

    setAxisXRange(minX, maxX);
}

void SurfaceGraph::adjustXMax(int max)
{
    double m_stepX = graph->getStepX();
    double m_rangeMinX = graph->getInfX();
    float maxX = m_stepX * float(max) + m_rangeMinX;

    int min = m_axisMinSliderX->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderX->setValue(min);
    }
    float minX = m_stepX * min + m_rangeMinX;

    setAxisXRange(minX, maxX);
}

void SurfaceGraph::adjustZMin(int min)
{
    double m_stepZ = graph->getStepZ();
    double m_rangeMinZ = graph->getInfZ();
    float minZ = m_stepZ * float(min) + m_rangeMinZ;

    int max = m_axisMaxSliderZ->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderZ->setValue(max);
    }
    float maxZ = m_stepZ * max + m_rangeMinZ;

    setAxisZRange(minZ, maxZ);
}

void SurfaceGraph::adjustZMax(int max)
{
    double m_stepZ = graph->getStepZ();
    double m_rangeMinZ = graph->getInfZ();
    float maxX = m_stepZ * float(max) + m_rangeMinZ;

    int min = m_axisMinSliderZ->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderZ->setValue(min);
    }
    float minX = m_stepZ * min + m_rangeMinZ;

    setAxisZRange(minX, maxX);
}
