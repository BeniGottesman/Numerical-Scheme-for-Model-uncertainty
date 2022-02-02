#include "nsmualgorithm.h"

double NSMUAlgorithm::execute(ParamLD &param)
{

    double S0 = param.getParameter("S0");
    double Rate = param.getParameter("Rate");
    double sigma_inf = param.getParameter("Sigma_inf");
    double sigma_sup = param.getParameter("Sigma_sup");
    double T = param.getParameter ("T");
    //this->getPayoff ()->setMaturity (T);
    int N = (int)param.getParameter ("N");//50;//
    //int N = (int) N_double;

    double p, tmp_exp;
    double tmp_f, tmp_g, tmp_pmax;
    double discount;

    double Solution;

    double ** V;
    V = (double **) malloc(sizeof(double *) * (N+1));//from 0 to N = N+1 values
    if (V == NULL)
        exit (1);
    for (int i = 0; i <= N; i++)
    {
        V[i] = (double *) malloc(sizeof(double) * (2*N+1));//from -N to N and +1 for 0
        if (V[i] == NULL)
            exit (1);
    }

    //Boundary condition
    discount = exp(-Rate*T);
    for(int K = -N; K <= N; K++)
    {
        y = S0*exp(sigma_sup*sqrt(T/(double)N)*(double)K)/discount;
        tmp_f = this->getPayoff ()->valueBuyer(y, T);//f
        V[N][N+K] = discount*tmp_f;
        //std::cout << " V["<<N<<"]["<<K<<"] = "<< V[N][N+K]<<"\n";
    }

    //Backward Induction
    p = exp(-4.0*sigma_sup*sqrt(T/(double)N))*(sigma_inf*sigma_inf)/(sigma_sup*sigma_sup);
    tmp_exp = exp(sigma_sup*sqrt(T/(double)N));
    for(int j = N-1; j>=0; j--)
    {
        discount = exp(-Rate*T*(double)j/(double)N);
        for(int K=-j; K<=j; K++)
        {
            y = S0*exp(sigma_sup*sqrt(T/(double)N)*(double)K)/discount;

            tmp_f = discount*this->getPayoff ()->valueBuyer(y); //f
            tmp_g = discount*this->getPayoff ()->valueSeller(y); //g

            double tmp_max_p = (1.0f-p)*V[j+1][N+K] + p/(1.0f+tmp_exp)*V[j+1][N+K+1] + p*tmp_exp/(1.0f+tmp_exp)*V[j+1][N+K-1];
            double tmp_max_1 = 1.0f/(1.0f+tmp_exp)*V[j+1][N+K+1] + tmp_exp/(1.0f+tmp_exp)*V[j+1][N+K-1];

            tmp_pmax = std::max (tmp_max_p, tmp_max_1);

            V[j][N+K] = std::max (tmp_f, std::min (tmp_g, tmp_pmax));

            //Output debug
            //std::cout << " V["<<j<<"]["<<K<<"] = "<< V[j][N+K]<<"\n";
        }
    }

    //std::cout << "Simulation 1 : V[0][0] = "<< V[0][N]<<"\n";
    //ui->Solution->setText(QString::number(V[0][N]));
    Solution = V[0][N];

    for (int i = 0; i <= N; i++)
        std::free(V[i]);
    std::free(V);

    return Solution;
}
