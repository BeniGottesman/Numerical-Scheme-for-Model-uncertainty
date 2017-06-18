#ifndef GRAPH3D_H
#define GRAPH3D_H

#include <QString>
#include "Graph/graph.h"
#include "Parameter/parameter.h"



// == typedef Graph < ParamLD, Q3DSurface > G3DSP;
class Graph3D : public G3DSP
{
public:
    Graph3D ();
    Graph3D (QString String_X, QString String_Y, QString String_Z,
             QString label_X, QString label_Z,
             double subdivision_X, double subdivision_Z,
             double min_X, double max_X,
             double min_Z, double max_Z);
             //double maxYValue = 1);
    Graph3D (Graph3D & );
    void updateValue(QString key, unsigned int i);

    void updateParameter (QString key, double Value);

    double getStepX () {return stepX;}
    double getStepZ () {return stepZ;}

    double getInfX () {return infX;}
    double getInfZ () {return infZ;}

    double getSupX () {return supX;}
    double getSupZ () {return supZ;}

    double getSubdX () {return subdivisionX;}
    double getSubdZ () {return subdivisionZ;}

    /*double getX () {return indexX;}
    double getZ () {return indexZ;}*/

    QString getLabelX () {return labelX;}
    QString getLabelZ () {return labelZ;}

private:
    double stepX, stepZ;
    double infX, infZ;
    double supX, supZ;
    double indexX, indexZ;
    unsigned int subdivisionX, subdivisionZ;//redundant

    QString labelX, labelZ;
};

#endif // GRAPH3D_H
