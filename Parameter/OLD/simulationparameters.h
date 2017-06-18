#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QString>
#include <QVector>
#include <QMap>

#include "parameter.h"

//Pattern Composite?

//Manage also the conflict between division and Parameter
//if already existed
class SimulationParameters
{
public:
    SimulationParameters (){}
    SimulationParameters (const DivisionList * divlist,
                          const ParamLD * paramList);
    SimulationParameters (SimulationParameters &sp);


    void addDivision (QString dName, Division d);
    void setAParameterValue(QString pName, double p);

    void addParameter (QString pName, double p);
    void addParameter (QString pName, ParamD p)
    {
        //if pName already exists with a value
        //it is replaced by value p
        pl.addParameter(pName, &p);
    }

    double getParameterValue (QString pName);
    void setParameterValue (QString pName, double * pNum);
    void setParameter (QString pName, ParamD * pNum);

    void setCursorDivAt (QString axe, unsigned int i);
    void setParameterDivAt (QString axe, unsigned int i);

    void removeDivision (QString div);
    void removeAnElement (QString param);

    DivisionList * getDivisionlist ();
    ParamLD getParameterList ();

protected:
    DivisionList dl;
    ParamLD pl;
};

typedef SimulationParameters SP;

/*class financialOptionParameters : public SimulationParameters
{
public:
    financialOptionParameters (double S0, double Rate,
                               double VolatilityInf, double VolatilitySup, double T, double Penalty,
                               unsigned int N = 1);

    double getS0 () {/*if (map.contains("S0"))*//*return *(pl ["S0"]->getValue());}
/*    double getRate () {return *(pl ["Rate"]->getValue());}
    double getVolatilityInf () {return *(pl ["VolatilityInf"]->getValue());}
    double getVolatilitySup () {return *(pl ["VolatilitySup"]->getValue());}
    double getPenalty () {return *(pl ["Penalty"]->getValue());}
    double getMaturity () {return *(pl ["T"]->getValue());}

    double setS0 (double S0) {pl ["S0"]->setValue(&S0);}
    double setRate (double Rate) {pl ["Rate"]->setValue(&Rate);}
    double setVolatilityInf (double VolInf) {pl ["VolatilityInf"]->setValue(&VolInf);}
    double setVolatilitySup (double VolSup) {pl ["VolatilitySup"]->setValue(&VolSup);}
    double setPenalty (double Penalty) {pl ["Penalty"]->setValue(&Penalty);}
    double setMaturity (double T) {pl ["T"]->setValue(&T);}

private:

};
*/
#endif // PARAMETERS_H
