#include "simulationparameters.h"

SimulationParameters::SimulationParameters (const DivisionList *divlist,
                                            const ParamLD *paramList)
{
    this->pl = *paramList;
    this->dl = *divlist;
}

SimulationParameters::SimulationParameters (SimulationParameters &sp)
{
    this->dl = *(sp.getDivisionlist());
    this->pl = sp.getParameterList();
}

void SimulationParameters::addDivision (QString dName, Division d)
{
    double newValue = d.getParameterAt(0);

    dl.addDivision(dName, d);

    pl.setParameterValue(dName, newValue);
}

void SimulationParameters::setCursorDivAt (QString axe,
                                            unsigned int i)
{
    double newvalue = dl.getValueOf (axe);
    QString param = dl.getLabelOfAnAxe(axe);
    dl.setAxeDivAt(axe, i);

    pl.setParameterValue(param, newvalue);

}

void SimulationParameters::addParameter (QString pName, double p)
{
    ParamD * tmpParam = new ParamD (&p, pName);
    pl.addParameter(pName, tmpParam);
}


double SimulationParameters::getParameterValue (QString pName)
{
    return pl.getParameter(pName);
}

void SimulationParameters::setParameterValue(QString pName, double *p)
{
    pl.setParameterValue(pName, *p);
}

void SimulationParameters::setParameter (QString pName, ParamD * pNum)
{
    pl.setParameter(pName, pNum);
}

void SimulationParameters::setParameterDivAt (QString axe, unsigned int i)
{
    dl.setAxeDivAt(axe, i);

    double newValue = dl.getValueOf(axe);
    QString pName = dl.getLabelOfAnAxe(axe);
    pl.setParameterValue(pName, newValue);
}

void SimulationParameters::removeAnElement (QString param)
{
    pl.removeParameter(param);

    QString axe_tmp = dl.getDivisionAxe(param);
    dl.removeDivision(axe_tmp);
}

DivisionList * SimulationParameters::getDivisionlist()
{
    return &dl;
}

ParamLD SimulationParameters::getParameterList ()
{
    return pl;
}


////////////////
////////////////
/// ////////////
/*financialOptionParameters::financialOptionParameters (
        double S0, double Rate,
        double VolatilityInf, double VolatilitySup,
        double T, double Penalty,
        unsigned int N)
{
    this->addParameter("S0", S0);
    this->addParameter("Rate", Rate);
    this->addParameter("VolatilityInf", VolatilityInf);
    this->addParameter("VolatilitySup", VolatilitySup);
    this->addParameter("T", T);
    this->addParameter("Penalty", Penalty);

    this->N = N;
}
*/
