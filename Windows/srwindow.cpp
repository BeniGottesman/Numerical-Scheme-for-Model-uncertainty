#include "srwindow.h"
#include "ui_srwindow.h"

SRWindow::SRWindow(ParamLD *pLD,
                   PayoffAlgorithm *NSMU,
                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SRWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Stopping Region");

    ui->graphPlot->addGraph ();
    /*ui->graphPlot->graph(0)->setPen(QPen(QColor(255, 100, 0)));
    ui->graphPlot->graph(0)->setBrush(QBrush()); // fill with texture of specified image
    ui->graphPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->graphPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));*/
    ui->graphPlot->graph(0)->setName("Stopping Region");

    T = QVector <double> (0);
    root_St = QVector <double> (0);

    // create graph and assign data to it:

    ///////FIRST PLOT
    ui->graphPlot->addGraph();
    QPen blueDotPen;
    blueDotPen.setColor(QColor(0, 0, 255, 150));
    blueDotPen.setStyle(Qt::DotLine);
    blueDotPen.setWidthF(2);
    ui->graphPlot->graph(0)->setPen(blueDotPen);

    ComputeStoppingRegion (pLD, NSMU);

    ui->graphPlot->graph(0)->setData(T, root_St);
    ///////FIRST PLOT

    ////////////////SECOND PLOT////////////////
    ui->graphPlot->addGraph();
    QPen OrangeDotPen;
    OrangeDotPen.setColor(QColor(255, 127, 80, 150));
    OrangeDotPen.setStyle(Qt::SolidLine);
    OrangeDotPen.setWidthF(2);
    ui->graphPlot->graph(1)->setPen(OrangeDotPen);
    pLD->setParameterValue("Sigma_inf", 0);

//    double tmp = root_St[5];
    T.clear();root_St.clear();

    ComputeStoppingRegion (pLD, NSMU);
//std::cout << tmp-root_St [5] << std::endl;
    ui->graphPlot->graph(1)->setData(T, root_St);
    ////////////////SECOND PLOT////////////////

    ////////////////Third PLOT////////////////
    ui->graphPlot->addGraph();
    QPen GreenDotPen;
    GreenDotPen.setColor(QColor(0, 255, 0, 150));
    GreenDotPen.setStyle(Qt::DashDotLine);
    GreenDotPen.setWidthF(2);
    ui->graphPlot->graph(2)->setPen(GreenDotPen);
    pLD->setParameterValue("Sigma_inf", 0.2);
    pLD->setParameterValue("Sigma_sup", 0.2);

//    double tmp = root_St[5];
    T.clear();root_St.clear();

    ComputeStoppingRegion (pLD, NSMU);
//std::cout << tmp-root_St [5] << std::endl;
    ui->graphPlot->graph(2)->setData(T, root_St);
    ////////////////Third PLOT////////////////


/*    ////////////////4 PLOT////////////////
    ui->graphPlot->addGraph();
    QPen BrownDotPen;
    BrownDotPen.setColor(QColor(139, 69, 19, 150));
    BrownDotPen.setStyle(Qt::DashDotDotLine);
    BrownDotPen.setWidthF(2);
    ui->graphPlot->graph(3)->setPen(BrownDotPen);
    pLD->setParameterValue("Sigma_sup", 0.2);

//    double tmp = root_St[5];
    T.clear();root_St.clear();

    ComputeStoppingRegion (pLD, NSMU);
//std::cout << tmp-root_St [5] << std::endl;
    ui->graphPlot->graph(3)->setData(T, root_St);
    ////////////////4 PLOT////////////////*/


    // give the axes some labels:
    // set axes ranges, so we see all data:
    //double Tinf = pLD->getParameter("T");
    double Tsup = pLD->getParameter("T_sup");
    ui->graphPlot->xAxis->setLabel("Maturity");
    ui->graphPlot->xAxis->setRange(T[0], Tsup);

    double St_inf = pLD->getParameter("St_inf");
    double St_sup = pLD->getParameter("St_sup");
    ui->graphPlot->yAxis->setRange(St_inf, St_sup);
    ui->graphPlot->yAxis->setLabel("Stock Value");

    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(ui->graphPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->graphPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->graphPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->graphPlot->yAxis2, SLOT(setRange(QCPRange)));

    ui->graphPlot->replot();

    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    ui->graphPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    this->show ();
}

void SRWindow::ComputeStoppingRegion(ParamLD *pLD,
                                     PayoffAlgorithm *NSMU)
{
    int i = 0, j = 0;
    double T_tmp = pLD->getParameter("T");
    double Tmin = pLD->getParameter("T");
    double Tdiv = pLD->getParameter("Tdiv");
    double Tmax = pLD->getParameter("T_sup");
    double step = (Tmax-Tmin)/Tdiv;

    for (i = 0; Tmin + i*step <= Tmax; i++)
    {
        pLD->setParameterValue("T", Tmin + i*step);
        double tmp_sol = SolvingRoot (pLD, NSMU);
        if (tmp_sol != -1)
        {
            T.push_back(Tmin + i*step);
            root_St.push_back(tmp_sol);
            j++;
        }
    }
    pLD->setParameterValue("T", T_tmp);
}

//http://www.codewithc.com/c-program-for-bisection-method/
double SRWindow::SolvingRoot (ParamLD *pLD,
                              PayoffAlgorithm *NSMU)
{
    double root_St = -1;

    double S0_save = pLD->getParameter("S0");
    double St_inf;// = pLD->getParameter("St_inf");
    double St_sup = pLD->getParameter("St_sup");

    int itr = 0, maxmitr = 200;//max iterations
    double x;
    double Precision = pLD->getParameter("Precision");
    //printf("\nEnter the values of a, b, allowed error and maximum iterations:\n");
    //scanf("%f %f %f %d", &St_inf, &St_sup, &allerr, &maxmitr);
    bisection (&x, St_inf, St_sup, &itr);
    do
    {
        //Criterion
        //V00-IP <= 0
        pLD->setParameterValue("S0", St_inf);
        double V00_Inf = NSMU->execute(*pLD);
        double IP_Inf = NSMU->getPayoff()->valueSeller(St_inf);//IP = intinsic Payoff
        double tmp_Inf = V00_Inf - IP_Inf;

        pLD->setParameterValue("S0", x);
        double V00_x = NSMU->execute(*pLD);
        double IP_x = NSMU->getPayoff()->valueSeller(x);//IP = intinsic Payoff
        double tmp_x = V00_x - IP_x;

        if (tmp_Inf*tmp_x > 0)
        {
            //pLD->setParameterValue ("St_inf", x);
            St_inf = x;
        }
        else if (tmp_Inf*tmp_x == 0)
        {
            if (tmp_Inf == 0 && tmp_x == 0)
            {
                root_St = pLD->getParameter ("S0");
                pLD->setParameterValue("S0", S0_save);
                return root_St;
            }
            else //tmp_x == 0 && tmp_inf < 0
            {
                //pLD->setParameterValue ("St_sup", x);
                St_sup = x;
            }
        }

        bisection (&root_St, St_inf, St_sup, &itr);
        //if I am inside the ball
        if (fabs(root_St-x) < Precision && -tmp_x < Precision)
        {
            pLD->setParameterValue("S0", S0_save);
            return root_St;
        }
        x = root_St;
    }
    while (itr < maxmitr);
    pLD->setParameterValue("S0", S0_save);
    return -1;//root_St;
}

void SRWindow::bisection (double *x, double inf, double sup, int *itr)
{
    *x=(inf+sup)/2;
    ++(*itr);
}

SRWindow::~SRWindow()
{
    delete ui;
}
