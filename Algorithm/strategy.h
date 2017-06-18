#ifndef STRATEGY_H
#define STRATEGY_H

#include <string>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <QtDataVisualization>
#include <Q3DSurface>
#include <QSurfaceDataProxy>

#include "Parameter/parameter.h"
#include "option.h"

using namespace QtDataVisualization;

class AbstractAlgorithm
{
public :

};

class PayoffAlgorithm : AbstractAlgorithm
{
public:
    PayoffAlgorithm (Payoff &po) { this->po = &po; }

    virtual double execute (ParamLD &p) = 0;
    Payoff * getPayoff () { return po; }

private:
    Payoff * po;
};


class Strategy
{
private:
    PayoffAlgorithm * AbsStrategy;

public:
    Strategy() {}

    Strategy(PayoffAlgorithm * strategy) : AbsStrategy(strategy)
    {
    }

    Strategy(Strategy & strategy)
    {
        this->AbsStrategy = strategy.getStrategy();
    }

    PayoffAlgorithm * getStrategy()
    {
        return AbsStrategy;
    }

    double execute(ParamLD &p)
    {
        double V00 = AbsStrategy->execute (p);
        return V00;
    }
};

#endif // STRATEGY_H
