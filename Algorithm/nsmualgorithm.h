#ifndef NSMUALGORITHM_H
#define NSMUALGORITHM_H
#include "strategy.h"

#include <cstdio>
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "MathParser/exprtk.hpp"
#include "Parameter/parameter.h"

class NSMUAlgorithm : public PayoffAlgorithm
{
public:
    NSMUAlgorithm (Payoff &po):PayoffAlgorithm(po){}

    double execute (ParamLD &param);

private:
    typedef exprtk::symbol_table<double> symbol_table_t;
    typedef exprtk::expression<double>     expression_t;
    typedef exprtk::parser<double>             parser_t;
    expression_t expression_fxy;
    expression_t expression_gxy;
    expression_t expression_hxy;

    double y;
    symbol_table_t symbol_table;

};

#endif // NSMUALGORITHM_H
