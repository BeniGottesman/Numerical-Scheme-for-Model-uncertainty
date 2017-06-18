#include "division.h"

Division::Division(Parameter<double> * p,
                   double parameter_inf, double parameter_sup,
                   unsigned int subdivision,
                   QString labelDiv, QString Axe)
{
    double tmpStep;

    this->theParameter = p;
    this->divInf.setValue(&parameter_inf);
    this->divSup.setValue(&parameter_sup);
    this->subdivision = subdivision;
    tmpStep = (parameter_sup - parameter_inf)/subdivision;
    this->Step.setValue(&tmpStep);

    this->labelDivision = labelDiv;

    this->Axe = Axe;
}

Division::~Division()
{
    delete theParameter;
}

Division::Division (Division &d)
{
    double param_tmp = d.getValue();
    double step_tmp = d.getStep ();
    double divInf_tmp = d.getDivisionInf();
    double divSup_tmp = d.getDivisionSup();
    QString pName_tmp = d.getParameterName();

    this->theParameter = new ParamD(&param_tmp, pName_tmp);
    this->divInf = ParamD(&divInf_tmp, pName_tmp);
    this->divSup = ParamD(&divSup_tmp, pName_tmp);
    this->Step = ParamD (&step_tmp, pName_tmp);
    this->subdivision = d.getSubdivision();
    this->labelDivision = d.getLabelDivision();
    this->Axe = d.getAxe();
}

double Division::getParameterAt(unsigned int i)
{
    if(i <= subdivision)
    {
        double tmpStep = *(Step.getValue());
        double tmpDivInf = *(divInf.getValue());
        return tmpDivInf + i*tmpStep;
    }
    return -1;//QException
}

void Division::setCursorAt(unsigned int i)
{
    double tmpParam_i = getParameterAt(i);
    theParameter->setValue(&tmpParam_i);
}


/// ///////////////////
///////////DivisionList
/// ///////////////////
void DivisionList::setAxeDivAt (QString axe, unsigned int i)
{
    if (divisionList.contains(axe))//Exception
    {
        double newValue;
        divisionList[axe]->setCursorAt(i);
        newValue = divisionList[axe]->getParameterAt(i);
    }
}


void DivisionList::addDivision (QString dName, Division d)
{
    if (divisionList.contains(dName))//the axe already exist
    {
        //we cannot plot 2 times the same parameter
        divisionList.remove(dName);
    }
    //the axe is already registered ?
    //-> We delete the division associated
    QMapIterator<QString, Division *> i(divisionList);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->getAxe() == d.getAxe())
        {
            divisionList.remove (i.key());
        }
        break;//pty of bijection
    }
    // /!\ faire gaffe : I have not to insert sigma and T in axe 0z
    //One axe for one division
    //we overwrite
    divisionList.insert (dName, &d);
}

QString DivisionList::getDivisionAxe (QString pName)
{
    foreach (Division * tmp_div, divisionList)
        if (tmp_div->getLabelDivision() == pName)
            return tmp_div->getAxe();
    return QString ("");
}

QString DivisionList::getLabelOfAnAxe (QString axe)
{
    return divisionList [axe]->getLabelDivision();
}
