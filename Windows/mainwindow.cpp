#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_StoppingRegionButton_clicked()
{
    /*Value from the GUI*/
    double St_inf = ui->St_inf->value();
    double St_sup = ui->St_sup->value();
    double Precision = pow(10,-1*ui->Precision->value());
    double Rate = ui->rate_SR->value();
    double Sigma_inf = ui->Sigma_inf_SR->value();
    double Sigma_sup = ui->Sigma_sup_SR->value();
    double T_sup = ui->T_sup_SR->value();
    double Tdiv = ui->Tdiv_SR->value();
    double N = ui->N_SR->value();
    /*Value from the GUI*/

    //Add a QException ?
    if (ui->OptionChoice->currentText() == "Choose a Payoff")
    {
        QMessageBox::critical(this, "Problem", "Choose a Payoff");
        return;
    }
    if (Sigma_sup < Sigma_inf)
    {
        QMessageBox::critical(this, "Problem", "Problem : sigma_sup < sigma_inf");
        return;
    }
    if (St_sup < St_inf)
    {
        QMessageBox::critical(this, "Problem", "Problem : S0_sup < S0_inf");
        return;
    }

    Payoff * po;
    if (ui->OptionChoice->currentText() == "C\+\+ Code")
    {
        std::string fxy = ui->fxy->toPlainText().toLocal8Bit().constData();
        std::string gxy = ui->gxy->toPlainText().toLocal8Bit().constData();
        std::string hxy = ui->hxy->toPlainText().toLocal8Bit().constData();

        po = new Payoff (new CPPAlgorithm (fxy, gxy, hxy));
    }
    else
    {
        double Strike = ui->Strike->value();
        double Penalty = ui->Penalty->value();

        if (ui->OptionChoice->currentText() == "American Call" ||
                ui->OptionChoice->currentText() == "American Put")
            Penalty = 100000000000.0;

        if (ui->OptionChoice->currentText() == "Game Call" ||
                ui->OptionChoice->currentText() == "American Call")
        {
            po = new Payoff (new Call (Strike, Penalty));
            //abstractAlgo = new CallAlgorithm (Strike, Penalty);
        }
        else if (ui->OptionChoice->currentText() == "Game Put" ||
                 ui->OptionChoice->currentText() == "American Put")
        {
            po = new Payoff (new Put (Strike, Penalty));
            //abstractAlgo = new PutAlgorithm (Strike, Penalty);
        }
    }

    PayoffAlgorithm * abstractAlgo = new NSMUAlgorithm (*po);
    ParamLD pLD;
    double zero = 0;
    pLD.addParameter("S0", &St_inf);
    pLD.addParameter("St_inf", &St_inf);
    pLD.addParameter("St_sup", &St_sup);
    pLD.addParameter("Rate", &Rate);
    pLD.addParameter("Sigma_inf", &Sigma_inf);
    pLD.addParameter("Sigma_sup", &Sigma_sup);
    pLD.addParameter("T", &zero);
    pLD.addParameter("T_sup", &T_sup);
    pLD.addParameter("Tdiv", &Tdiv);
    pLD.addParameter("Precision", &Precision);
    pLD.addParameter("N", &N);

    //Here we know the axes so we don't have to pass through communicator
    SRW = new SRWindow (&pLD, abstractAlgo);
    this->hide();
}

void MainWindow::on_DoRecursion_clicked()
{
    /*Value from the GUI*/
    double S0_inf = ui->S0_inf->value();
    double S0_sup = ui->S0_sup->value();
    double S0div = ui->S0div->value();
    double Rate = ui->rate->value();
    double Sigma_inf = ui->Sigma_inf->value();
    double Sigma_sup = ui->Sigma_sup->value();
    double T_inf = ui->T_inf->value();
    double T_sup = ui->T_sup->value();
    double Tdiv = ui->Tdiv->value();
    double N = ui->N->value();
    /*Value from the GUI*/


    //Add a QException ?
    if (ui->OptionChoice->currentText() == "Choose a Payoff")
    {
        QMessageBox::critical(this, "Problem", "Choose a Payoff");
        return;
    }
    if (Sigma_sup < Sigma_inf)
    {
        QMessageBox::critical(this, "Problem", "Problem : sigma_sup < sigma_inf");
        return;
    }
    if (S0_sup < S0_inf)
    {
        QMessageBox::critical(this, "Problem", "Problem : S0_sup < S0_inf");
        return;
    }
    if (T_sup < T_inf)
    {
        QMessageBox::critical(this, "Problem", "Problem : T_sup < T_inf");
        return;
    }

    Payoff * po;
    if (ui->OptionChoice->currentText() == "C\+\+ Code")
    {
        std::string fxy = ui->fxy->toPlainText().toLocal8Bit().constData();
        std::string gxy = ui->gxy->toPlainText().toLocal8Bit().constData();
        std::string hxy = ui->hxy->toPlainText().toLocal8Bit().constData();

        po = new Payoff (new CPPAlgorithm (fxy, gxy, hxy));
    }
    else
    {
        double Strike = ui->Strike->value();
        double Penalty = ui->Penalty->value();
        double Maturity = T_sup;

        if (ui->OptionChoice->currentText() == "Convertible Bond")
        {
            if (Strike <= 1)
            {
                QMessageBox::critical(this, "Problem", "K <= 1");
                return;
            }

            double gamma = 0.1;
            po = new Payoff (new ConvertibleBond (Strike, Penalty, Maturity, gamma));
        }
        else
        {
            if (ui->OptionChoice->currentText() == "American Call" ||
                    ui->OptionChoice->currentText() == "American Put")
                Penalty = 100000000000.0;

            if (ui->OptionChoice->currentText() == "Game Call" ||
                    ui->OptionChoice->currentText() == "American Call")
            {
                po = new Payoff (new Call (Strike, Penalty));
                //abstractAlgo = new CallAlgorithm (Strike, Penalty);
            }
            else if (ui->OptionChoice->currentText() == "Game Put" ||
                     ui->OptionChoice->currentText() == "American Put")
            {
                po = new Payoff (new Put (Strike, Penalty));
                //abstractAlgo = new PutAlgorithm (Strike, Penalty);
            }
        }
    }

    PayoffAlgorithm * abstractAlgo = new NSMUAlgorithm (*po);
    ParamLD pLD;

    pLD.addParameter("S0", &S0_inf);
    pLD.addParameter("S0_inf", &S0_inf);
    pLD.addParameter("S0_sup", &S0_sup);
    pLD.addParameter("S0div", &S0div);
    pLD.addParameter("Rate", &Rate);
    pLD.addParameter("Sigma_inf", &Sigma_inf);
    pLD.addParameter("Sigma_sup", &Sigma_sup);
    pLD.addParameter("T", &T_inf);
    pLD.addParameter("T_inf", &T_inf);
    pLD.addParameter("T_sup", &T_sup);
    pLD.addParameter("Tdiv", &Tdiv);
    pLD.addParameter("N", &N);

    parameterListDC<Q3DSurface> pLDC (pLD);

    Graph3D * graph = new Graph3D ("Initial stock Value",
                                   "Option Value",
                                   "Maturity",
                                   "S0", "T",
                                   S0div, Tdiv,
                                   S0_inf,S0_sup,
                                   T_inf,T_sup);

    ICommunicator<ParamLD, Q3DSurface>::Connect(&pLDC, pLDC.getParameterList(),
                                                graph, graph->getGraph());

    DW = new DrawWindow (*(pLDC.getParameterList()), graph, abstractAlgo);
    this->hide();
}

void MainWindow::on_OptionChoice_currentIndexChanged(const QString &PayoffMenu)
{
    if (PayoffMenu == "C++ Code")
    {
        ui->OptionInputs->setEnabled(false);
        ui->CPPCode->setEnabled(true);
    }
    else if (PayoffMenu == "Convertible Bond")
    {
        ui->OptionInputs->setEnabled(true);
        ui->CPPCode->setEnabled(false);
        ui->Penalty->setEnabled(false);
    }
    else if (PayoffMenu == "Game Call" ||
             PayoffMenu == "Game Put")
    {
        //Open a new window and add values :
        //Strike and Penalty
        ui->OptionInputs->setEnabled(true);
        ui->CPPCode->setEnabled(false);
        ui->Penalty->setEnabled(true);
    }

    else if (PayoffMenu == "American Call" ||
             PayoffMenu == "American Put")
    {
        ui->OptionInputs->setEnabled(true);
        ui->CPPCode->setEnabled(false);
        ui->Penalty->setEnabled(false);
    }
    else
    {
        ui->OptionInputs->setEnabled(false);
        ui->CPPCode->setEnabled(false);
    }
}

/*void MainWindow::on_actionAbout_clicked ()
{

}*/
