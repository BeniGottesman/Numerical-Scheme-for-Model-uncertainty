#ifndef PARAMETER_H
#define PARAMETER_H

#include <QString>
#include <QMap>
#include <Pattern/Com/communicator.h>
#include <iostream>
//#include <Graph/graph.h>

template < typename T >
class Parameter
{
public:
    Parameter (){}
    Parameter (T * param, QString Name) :
        ParameterName(Name){p = param;}
    Parameter (QString Name, T * param) :
        ParameterName(Name){p = param;}
    Parameter (Parameter<T> &param)
    {
        this->p = param.getValue();
        this->ParameterName = param.getParameterName();
    }

    void operator= (Parameter & param)
    {
        this->p = param.getValue();
        this->ParameterName = param.getParameterName();
    }

    T * getValue () {return p;}
    void setValue (T * newValue) {*p = *newValue;}

    QString getParameterName () {return ParameterName;}
    void setParameterName (QString name){ParameterName = name;}

private:
    T * p;
    QString ParameterName;
};

//Composite
template < typename T >
class IParameterList
{
public:
    //Add an Exception
    IParameterList(){}
    IParameterList (QMap<QString, Parameter<T> *> *pL)
    {this->pL = *paramList;}
    IParameterList (IParameterList<T> &p)//{ this->pL = *(p.getParameterList()); }
    {
        QMap<QString, Parameter<T> *>::iterator i = p.getParameterList()->begin();
        while (i != p.getParameterList()->end())
        {
            addParameter(i.key(),i.value());
            ++i;
        }
    }
    ~IParameterList (){}//add a delete loop for <...Parameter<T> *>

    void addParameter (QString dName, T * p)
    {
        Parameter<T> * tmpParam = new Parameter<T> (p, dName);
        pL.insert(dName, tmpParam);
    }
    void addParameter (QString pName, Parameter<T> *p)
    {
        //if pName already exists with a value
        //it is replaced by value p
        pL.insert (pName, p);
    }
    void setParameter (QString pName, Parameter<T> * pNum)
    { pL[pName] = pNum; }

    void setParameterValue (QString pName, double p)
    { pL [pName]->setValue(&p); }

    QMap<QString, Parameter<T> *> * getParameterList (){return &pL;}
    T getParameter (QString pName){ return *(pL[pName]->getValue()); }
    QString getParameterLabel (QString pName) { return pL[pName]->getParameterName(); }

    void removeParameter (QString pName){ pL.remove(pName); }

private:
    QMap<QString, Parameter<T> *> pL; //add destructor for the pointer
};

typedef Parameter < double > ParamD;
typedef IParameterList < double > ParamLD;

template < typename GraphType >
class parameterListDC : public ICommunicator < ParamLD, GraphType > //D for double
{
public:
    parameterListDC () {}
    parameterListDC (ParamLD &pLD) //kind of copy constructor
    {this->pLD = new ParamLD (pLD);}
    parameterListDC (ICommunicator < ParamLD, GraphType > * c)
    {this->c = new CommunicatorChild (this, c->getT2 ());}
    ParamLD * getParameterList () {return pLD;}

private:
    ParamLD *pLD;
};

//We can make a specialisation/composite leaf
//for a special parameter like N (an integer)
//or gamma which belong to (0,1) for example



#endif // PARAMETER_H
