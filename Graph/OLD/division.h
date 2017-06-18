#ifndef DIVISION_H
#define DIVISION_H

#include <QString>
#include <QMap>
#include "Parameter/parameter.h"


class Division
{
public:
    Division(){}
    Division(ParamD * p,
             double parameter_inf, double parameter_sup,
             unsigned int subdivision,
             QString labelDiv = "", QString Axe = "z");
    Division (Division &d);

    ~Division ();

    double getStep () {return *(Step.getValue());}
    unsigned int getSubdivision () {return this->subdivision;}
    double getDivisionInf() {return *(divInf.getValue());}
    double getDivisionSup() {return *(divSup.getValue());}
    QString getParameterName () {return theParameter->getParameterName();}
    QString getLabelDivision () {return labelDivision;}
    double getValue () {return *(theParameter->getValue()); }
    double getParameterAt(unsigned int i);
    QString getAxe (){return Axe;}

    void setCursorAt(unsigned int i);


private:
    ParamD * theParameter;
    ParamD divInf;
    ParamD divSup;

    ParamD Step;
    unsigned int subdivision;//N

    QString labelDivision;
    QString Axe;
};

class DivisionList
{
public:
    DivisionList (){}
    DivisionList (QMap <QString, Division *> divisionList);

    void addDivision (QString dName, Division d);
    void removeDivision (QString div)
    { divisionList.remove(div); }

    DivisionList *getDivisionlist ()
    { return this; }
    double getValueOf(QString axe)
    { return divisionList[axe]->getValue(); }
    Division * getDivision (QString Axe)
    { return (divisionList[Axe]); }

    QString getLabelOfAnAxe (QString axe);
    QString getDivisionAxe (QString pName);

    void setAxeDivAt (QString axe, unsigned int i);

private:
    QMap <QString, Division *> divisionList;
};

#endif // DIVISION_H
