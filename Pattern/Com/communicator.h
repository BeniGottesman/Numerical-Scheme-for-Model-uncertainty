#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QMap>
#include <QString>

template <typename T1, typename T2>
class Communicator
{
public:
    Communicator () {t1 = NULL, t2 = NULL;}
    Communicator (T1 *t1) {this->t1 = t1;}
    Communicator (T2 *t2) {this->t2 = t2;}
    Communicator (T1 *t1, T2 *t2)
    {this->t1 = t1, this->t2 = t2;}//We connect T1 and T2

    ~Communicator () {}

    void setT1 (T1 *newt1) {
        t1 = newt1;
    }//by reference
    void setT2 (T2 *newt2) {t2 = newt2;}

    T1 *getT1() {return t1;}
    T2 *getT2() {return t2;}

    void removeT1 (){delete t1;}
    void removeT2 (){delete t2;}

    //virtual void const NotifyT1 (QString key, double Value) = 0; //we can take another type instead of double + cascade modification
    //void const NotifyT2 (double Value) = 0;

    void const addT1 (T1 *newT1) {t1 = newT1;}
    void const addT2 (T2 *newT2) {t2 = newT2;}

    //void const remove (QString key) = 0;

private:
    T1 *t1;
    T2 *t2;
};

template < typename T1, typename T2 >
class CommunicatorChild : public Communicator < T1, T2 >
{
public:
    CommunicatorChild () {}
    ~CommunicatorChild () {} //It is T1 and T2 which delete t1 and t2 and not this class

    void const addToT1 (QString key, double Value)
    {
        if (!t1.getParameterList().contains(key))
            return;
        t1.add (key, Value);
    }
    //void const NotifyT1 (QString key, T1 *Value)
    //{t1.updateParameter (key, Value);}

    //void const remove (QString key) {t2.remove(key);}
};

/////////////////////////////
/// \brief The ICommunicator<T1, T2> class
///
template < typename T1, typename T2 >
class ICommunicator
{
public:
    ICommunicator (){this->c = NULL;}
    ICommunicator (T1 *pl){this->c->setT1 (pl);}
    ICommunicator (T2 *g){this->c->setT2 (g);}
    ICommunicator (T1 *pl, T2 *g)
    {this->c->setT1 (pl);this->c->setT2 (g);}
    ICommunicator (CommunicatorChild <T1, T2> &com)
    {this->c = &com;}//by reference

    ~ICommunicator () {if (c != NULL) delete c;}//Only if I use new for a pointer

    CommunicatorChild <T1, T2> * const getCommunicator () {return c;}

    //addNewConnection
    void addNewConnection (){}//c = new CommunicatorChild < T1, T2 > ()
    void const addNewConnection (T1 *pl, T2 *g){c = new CommunicatorChild < T1, T2 > (pl, g);}
    void const addNewConnection (CommunicatorChild < T1, T2 > &com)
    {c = &com;}
    void const addNewConnection (T1 *pl)
    {
        if (c == NULL)
            c = new ICommunicator < T1, T2 > (pl, this);
        else this->c->setT1 (pl);
    }
    void const addNewConnection (T2 *g)//add a loop in T2 which ensures that g belong to 'this'
    {
        if (c == NULL)
            c = new ICommunicator < T1, T2 > (this, g);
        else this->c->setT2 (g);
    }

    //addNewConnection
    //template < typename T1, typename T2 >
    static void Connect (ICommunicator <T1, T2> *com1, T1 *t1,
                         ICommunicator <T1, T2> *com2, T2 *t2)
    {
        //CommunicatorChild <T1, T2> c_tmp;// = new CommunicatorChild <T1, T2> ();
        CommunicatorChild <T1, T2> * c_tmp = new CommunicatorChild <T1, T2> ();
        //if(com1!=com2)
        com1->addNewConnection(*c_tmp);
        com2->addNewConnection(*c_tmp);
        c_tmp->setT1 (t1);
        c_tmp->setT2 (t2);
    }

    T1 *getT1 () {return c->getT1 ();}
    T2 *getT2 () {return c->getT2 ();}

protected:
    CommunicatorChild <T1, T2> *c;
};

#endif // COMMUNICATOR_H
