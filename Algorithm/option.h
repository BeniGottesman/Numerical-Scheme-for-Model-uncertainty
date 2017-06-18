#ifndef OPTION
#define OPTION

#include <QString>
#include <cstdio>
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "MathParser/exprtk.hpp"

class AbstractPayoff
{
public:
    virtual double valueBuyer (double s) = 0;
    virtual double valueSeller (double s) = 0;
    virtual double hValue (double s) = 0;

    virtual void setMaturity (double Maturity) {}

    virtual double valueBuyer (double s, double time)
    {return valueBuyer (s);}

    virtual double valueSeller (double s, double time)
    {return valueSeller (s);}

    virtual QString getOptionType () = 0;
};

class AbstractOption : public AbstractPayoff
{
public:
    AbstractOption (double Strike = 0,
                    double Penalty = 100000000.0,
                    double Maturity = -1.0)
    {
        this->Strike = Strike;
        this->Penalty = Penalty;
        this->Maturity = Maturity;
    }
    virtual void setMaturity (double Maturity) { this->Maturity = Maturity;}

protected:
    double Strike;
    double Penalty;
    double Maturity;
};

class Call : public AbstractOption
{
public:
    Call (double Strike, double Penalty):AbstractOption(Strike,Penalty)
    {}

    virtual QString getOptionType ()
    {
        return QString ("Call");
    }

    double valueBuyer (double s)
    {
        if (s > Strike)
            return s-Strike;
        return 0;
    }

    double valueSeller (double s)
    {
        if (s > Strike)
            return s-Strike+Penalty;
        return Penalty;
    }
    double valueSeller (double s, double time)
    {
        if (s > Strike)
        {
            if (time != Maturity)
                return s-Strike+Penalty;
            else
                return (s - Strike);
        }
        return Penalty;
    }

    double hValue (double s)
    {
        return 0;
    }
};


class Put : public AbstractOption
{
public:
    Put (double Strike, double Penalty):AbstractOption(Strike,Penalty)
    {}

    virtual QString getOptionType ()
    {
        return QString ("Put");
    }


    double valueBuyer (double s)
    {
        if (s < Strike)
            return Strike-s;
        return 0;
    }

    double valueSeller (double s)
    {
        if (s < Strike)
            return Strike-s+Penalty;
        return Penalty;
    }
    double valueSeller (double s, double time)
    {
        if (s < Strike)
        {
            if (time != Maturity)
                return Strike-s+Penalty;
            else
                return Strike - s;
        }
        return Penalty;
    }

    double hValue (double s)
    {
        return 0;
    }
};

class ConvertibleBond : public AbstractOption
{
public:
    ConvertibleBond (double Strike, double Penalty,
                     double Maturity, double gamma)
        :AbstractOption(Strike, Penalty, Maturity)
    {
        if (gamma < 1 && gamma > 0)
            this->gamma = gamma;
        else this->gamma = 0;//send an Exception
    }

    virtual QString getOptionType ()
    {
        return QString ("Convertible Bond");
    }

    double valueBuyer (double s)
    { return gamma * s; }

    double valueBuyer (double s, double time)
    {
        if (time != this->Maturity)
            return gamma * s;
        return std::max (1.0, gamma * s);
    }

    double valueSeller (double s)
    { return std::max (Strike, gamma*s); }
    double valueSeller (double s, double time)
    {
        if (time != this->Maturity)
            return std::max (Strike, gamma*s);
        return std::max (1.0, gamma * s);
    }

    double hValue (double s)
    {
        return 0;
    }

private:
    double gamma;
};

class CPPAlgorithm : public AbstractPayoff
{
public:
    CPPAlgorithm::CPPAlgorithm(std::string fxy, std::string gxy, std::string hxy)
    {
        symbol_table.add_variable("y", y);

        expression_fxy.register_symbol_table(symbol_table);
        expression_gxy.register_symbol_table(symbol_table);
        expression_hxy.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(fxy,expression_fxy);
        parser.compile(gxy,expression_gxy);
        parser.compile(hxy,expression_hxy);
    }

    virtual QString getOptionType ()
    {
        return QString ("C++ code");
    }

    double valueBuyer (double s)
    {
        y = s;
        return expression_fxy.value ();
    }

    double valueSeller (double s)
    {
        y = s;
        return expression_gxy.value ();
    }

    double hValue (double s)
    {
        y = s;
        return expression_hxy.value ();
    }

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

class Payoff
{
private:
    AbstractPayoff * AbsOption;

public:
    Payoff() {}

    Payoff(AbstractPayoff * opt) : AbsOption(opt)
    {
    }

    double valueSeller(double s, double Maturity)
    {
        return AbsOption->valueSeller(s, Maturity);
    }
    double valueSeller(double s)
    {
        return AbsOption->valueSeller(s);
    }

    double valueBuyer(double s, double Maturity)
    {
        return AbsOption->valueBuyer(s, Maturity);
    }
    double valueBuyer(double s)
    {
        return AbsOption->valueBuyer(s);
    }

    double hValue(double s)
    {
        return AbsOption->hValue(s);
    }

    void setMaturity (double Maturity)
    {
        AbsOption->setMaturity(Maturity);
    }
};
#endif // OPTION

