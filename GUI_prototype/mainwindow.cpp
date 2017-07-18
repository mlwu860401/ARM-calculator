#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "backend.h"
#include <QString>
#include <QDebug>
#include <QCoreApplication>
#include <QDesktopWidget>
#include "glwidget.h"
#include <QApplication>
#include <QDoubleValidator>
#include <QHeaderView>
#include <QAbstractScrollArea>
#include <QLocale>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox_graphic->insertItem(0,"Amortization Chart");
    ui->comboBox_graphic->insertItem(1,"Annual Amortization Stack Chart");
    auto rate_input = new QDoubleValidator(0, 100, 3);
    ui->lineEdit_rate2->setValidator(rate_input);
    ui->lineEdit_rate3->setValidator(rate_input);
    ui->lineEdit_precentage->setValidator(rate_input);
    auto money_input = new QDoubleValidator(0, 100000000000000, 1);
    ui->lineEdit_prepay->setValidator(money_input);
    ui->tableWidget1->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->tableWidget2->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->tableWidget3->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->tableWidget_summary->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
}
MainWindow::~MainWindow()
{
    delete ui;
}
template <class T>
double MainWindow::inputget(QLineEdit *input, T output)
{
    bool ok;
    output = input->text().toDouble(&ok);
    if(TRUE && output > 0.0)
    {
        return output;
    }
    else
    {
        input->setText("ERR");
        return 0.0;
    }
}
template <class H>
bool MainWindow::Get(QLineEdit *input, H &output)
{
    bool ok;
    output = floor(input->text().toDouble(&ok));
    printf("output:%f\n", output);
    if(ok == true && output >= 1.0)
    {
        return true;
    }
    else
    {
        input->setText("ERR");
        output = 0.0;
        return false;
    }
}
bool MainWindow::loan_check(double loan)
{
    if(loan <= 0.0)
    {
        ui->lineEdit_loan->setText("ERR");
        return false;
    }
    else
    {
        return true;
    }
}
bool MainWindow::grace_check(double term, double grace)
{
    if(ui->lineEdit_grace->text() != "" && grace ==0)
    {
        return false;
    }
    if(grace >= term)
    {
        ui->lineEdit_grace->setText("ERR");
        return false;
    }
    else
    {
        return true;
    }
}
bool MainWindow::period_check(double term, double period2, double period3)
{
    printf("period2:%f, period3:%f\n", period2, period3);
    if(period2 == 0.0)
    {
        ui->lineEdit_period2->setText("ERR");
        return false;
    }
    if(period3 == 0.0)
    {
        ui->lineEdit_period3->setText("ERR");
        return false;
    }
    if(period2 == 0.0 && period3 != 0)
    {
        ui->lineEdit_period2->setText("ERR");
        ui->lineEdit_period3->setText("ERR");
        return false;
    }
    if(period2 > term || period3 > term)
    {
        if(period2 > term) {ui->lineEdit_period2->setText("ERR");}
        if(period3 > term) {ui->lineEdit_period3->setText("ERR");}
        return false;
    }
    else
    {
        if(period2 > period3)
        {
            ui->lineEdit_period2->setText("ERR");
            return false;
        }
        else
        {
            return true;
        }
    }
}
bool MainWindow::prepay_check(double term, double grace, QLineEdit *prepay_start, QLineEdit *prepay_end)
{
    if(prepay_start->text().toDouble() == 0.0 && prepay_start->text() != "")
    {
        prepay_start->setText("ERR");
        return false;
    }
    if(prepay_end->text().toDouble() == 0.0 && prepay_end->text() != "")
    {
        prepay_end->setText("ERR");
        return false;
    }
    if((prepay_start->text().toDouble() >= 1.0 && prepay_end->text().toDouble() == 0.0)|| (prepay_end->text().toDouble() >= 1.0 && prepay_start->text().toDouble() == 0.0))
    {
        prepay_start->setText("ERR");
        prepay_end->setText("ERR");
        return false;
    }
    else
    {
        if(grace != 0.0)
        {
            if(prepay_start->text().toDouble() > term || prepay_end->text().toDouble() > term || (prepay_start->text().toDouble() <= grace && prepay_start->text().toDouble() != 0) || (prepay_end->text().toDouble() <= grace && prepay_start->text().toDouble() != 0))
            {
                if(prepay_start->text().toDouble() > term) {prepay_start->setText("ERR");}
                if(prepay_end->text().toDouble() > term) {prepay_end->setText("ERR");}
                if(prepay_start->text().toDouble() <= grace) {prepay_start->setText("ERR");}
                if(prepay_end->text().toDouble() <= grace) {prepay_end->setText("ERR");}
                return false;
            }
            else
            {

                if(prepay_start->text().toDouble() > prepay_end->text().toDouble())
                {
                    prepay_start->setText("ERR");
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }
        else
            {
                if(prepay_start->text().toDouble() > term || prepay_end->text().toDouble() > term)
                {
                    if(prepay_start->text().toDouble() > term) {prepay_start->setText("ERR");}
                    if(prepay_end->text().toDouble() > term) {prepay_end->setText("ERR");}
                    return false;
                }
                else
                {

                    if(prepay_start->text().toDouble() > prepay_end->text().toDouble())
                    {
                        prepay_start->setText("ERR");
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
            }

    }
}
inline void MainWindow::filling_item(QTableWidget *tableWidget, QTableWidgetItem *table_ptr, int start, int end, double principal[], double interest[], double PMT[], double prepay_principal[], double prepay_interest[], double prepay_PMT[])
{
    QStringList table_index;
    QFont cell("Times", 10);
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
    for(int i = start - 1; i < end; i++)
    {
        table_index<<QString("%1").arg(i+1);
        for(int j = 0; j < 6; j++)
        {
            if(j==0)// assign princial
            {
                table_ptr = new QTableWidgetItem(QString("%L1").arg(principal[i], 0, 'f', 1));
            }
            if(j==1)
            {
                table_ptr = new QTableWidgetItem(QString("%L1").arg(interest[i], 0, 'f', 1));
            }
            if(j==2)
            {
                table_ptr = new QTableWidgetItem(QString("%1%").arg(interest[i]/PMT[i]*100.0, 0, 'f', 1));
            }
            if(j==3)
            {
                table_ptr = new QTableWidgetItem(QString("%L1").arg(prepay_principal[i], 0, 'f', 1));
            }
            if(j==4)
            {
                table_ptr = new QTableWidgetItem(QString("%L1").arg(prepay_interest[i], 0, 'f', 1));
            }
            if(j==5)
            {
                table_ptr = new QTableWidgetItem(QString("%1%").arg(prepay_interest[i]/prepay_PMT[i]*100.0, 0, 'f', 1));
            }
            tableWidget->setItem(i-start+1,j,table_ptr);
            table_ptr->setFont(cell);
        }
    }
    tableWidget->setVerticalHeaderLabels(table_index);

}
void MainWindow::clear_alltabs()
{
    ui->tableWidget1->clear();
    ui->tableWidget2->clear();
    ui->tableWidget3->clear();
    ui->tableWidget_summary->clear();
    ui->label_rate1->setText("");
    ui->label_term1->setText("");
    ui->label_PMT1->setText("");
    ui->label_sum1->setText("");
    ui->label_prepay_PMT1->setText("");
    ui->label_prepay_sum1->setText("");
    ui->tableWidget1->setColumnCount(0);
    ui->tableWidget1->setRowCount(0);
    ui->label_rate2->setText("");
    ui->label_term2->setText("");
    ui->label_PMT2->setText("");
    ui->label_sum2->setText("");
    ui->label_prepay_PMT2->setText("");
    ui->label_prepay_sum2->setText("");
    ui->tableWidget2->setColumnCount(0);
    ui->tableWidget2->setRowCount(0);
    ui->label_rate3->setText("");
    ui->label_term3->setText("");
    ui->label_PMT3->setText("");
    ui->label_sum3->setText("");
    ui->label_prepay_PMT3->setText("");
    ui->label_prepay_sum3->setText("");
    ui->tableWidget3->setColumnCount(0);
    ui->tableWidget3->setRowCount(0);
    ui->label_total_interest->setText("");
    ui->label_total_payment->setText("");
    ui->label_total_prepay_interest->setText("");
    ui->label_total_prepay_payment->setText("");
    ui->label_prepay_Iratio->setText("");
    ui->label_Iratio->setText("");
    ui->label_interest_saved->setText("");
    ui->tableWidget_summary->setColumnCount(0);
    ui->tableWidget_summary->setRowCount(0);
    ui->label_realtime_balance->setText("");
    ui->label_realtime_interest->setText("");
    ui->label_realtime_principal->setText("");
    ui->label_realtime_prepay_balance->setText("");
    ui->label_realtime_prepay_interest->setText("");
    ui->label_realtime_prepay_principal->setText("");
}
void MainWindow::Update_tab_period(int period_switch, int start, int end, double payment, double interest[], double principal[], double PMT[], double rate, double prepay_principal[], double prepay_interest[], double prepay_PMT[], double prepay_payment)
{
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
    int duration = end - start + 1;
    QStringList table_header;
    table_header<<"Principal"<<"Interest"<<"Interest Ratio"<<"Principal \n with Prepay"<<"Interest \n with Prepay"<<"Interest Ratio \n with Prepay";
    QTableWidgetItem *table_ptr = 0;
    if(period_switch == 1)
    {
        ui->tableWidget1->setColumnCount(6);
        //ui->tableWidget1->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tableWidget1->setHorizontalHeaderLabels(table_header);
        //setup row coount
        ui->tableWidget1->setRowCount(duration);
        //assign values
        ui->label_rate1->setText(QString::number(rate) + "%");
        ui->label_term1->setText(QString::number(start) + " ~ " + QString::number(end));
        ui->label_PMT1->setText(QString("$%L1 kNTD").arg(PMT[end-1]/1000.0));
        ui->label_sum1->setText(QString("$%L1 kNTD").arg(payment/1000.0));
        ui->label_prepay_PMT1->setText(QString("$%L1 kNTD").arg(prepay_PMT[end-1]/1000.0));
        ui->label_prepay_sum1->setText(QString("$%L1 kNTD").arg(prepay_payment/1000.0));
        filling_item(ui->tableWidget1, table_ptr, start, end, principal, interest, PMT, prepay_principal, prepay_interest, prepay_PMT);
    }
    if(period_switch == 2)
    {
        ui->tableWidget2->setColumnCount(6);
        //ui->tableWidget2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tableWidget2->setHorizontalHeaderLabels(table_header);
        //setup row coount
        ui->tableWidget2->setRowCount(duration);
        //assign values
        ui->label_rate2->setText(QString::number(rate) + "%");
        ui->label_term2->setText(QString::number(start) + " ~ " + QString::number(end));
        ui->label_PMT2->setText(QString("$%L1 kNTD").arg(PMT[end-1]/1000.0));
        ui->label_sum2->setText(QString("$%L1 kNTD").arg(payment/1000.0));
        ui->label_prepay_PMT2->setText(QString("$%L1 kNTD").arg(prepay_PMT[end-1]/1000.0));
        ui->label_prepay_sum2->setText(QString("$%L1 kNTD").arg(prepay_payment/1000.0));
        filling_item(ui->tableWidget2, table_ptr, start, end, principal, interest, PMT, prepay_principal, prepay_interest, prepay_PMT);
    }
    if(period_switch == 3)
    {
        ui->tableWidget3->setColumnCount(6);
        //ui->tableWidget3->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tableWidget3->setHorizontalHeaderLabels(table_header);
        //setup row coount
        ui->tableWidget3->setRowCount(duration);
        //assignvalues
        ui->label_rate3->setText(QString::number(rate) + "%");
        ui->label_term3->setText(QString::number(start) + " ~ " + QString::number(end));
        ui->label_PMT3->setText(QString("$%L1 kNTD").arg(PMT[end-1]/1000.0));
        ui->label_sum3->setText(QString("$%L1 kNTD").arg(payment/1000.0));
        ui->label_prepay_PMT3->setText(QString("$%L1 kNTD").arg(prepay_PMT[end-1]/1000.0));
        ui->label_prepay_sum3->setText(QString("$%L1 kNTD").arg(prepay_payment/1000.0));
        filling_item(ui->tableWidget3, table_ptr, start, end, principal, interest, PMT, prepay_principal, prepay_interest, prepay_PMT);
    }
}
inline void MainWindow::filling_item_summary(QTableWidget *tableWidget, QTableWidgetItem *table_ptr, int start, int end, double new_principal[], double new_interest[], double new_payment[], double new_prepay_principal[], double new_prepay_interest[], double new_prepay_payment[])
{
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
    QStringList table_index;
    QFont cell("Times", 10);
    for(int i = start - 1; i < end; i++)
    {
        table_index<<QString("%1").arg(i+1);
        for(int j = 0; j < 6; j++)
        {
            if(j==0)
            {
                table_ptr = new QTableWidgetItem(QString("%L1").arg(new_principal[i], 0, 'f', 1));
            }
            if(j==1)
            {
                table_ptr = new QTableWidgetItem(QString("%L1").arg(new_interest[i], 0, 'f', 1));
            }
            if(j==2)
            {
                table_ptr = new QTableWidgetItem(QString("%L1").arg(new_payment[i], 0, 'f', 1));
            }
            if(j==3)
            {
                table_ptr = new QTableWidgetItem(QString("%L1").arg(new_prepay_principal[i], 0, 'f', 1));
            }
            if(j==4)
            {
                table_ptr = new QTableWidgetItem(QString("%L1").arg(new_prepay_interest[i], 0, 'f', 1));
            }
            if(j==5)
            {
                table_ptr = new QTableWidgetItem(QString("%L1").arg(new_prepay_payment[i], 0, 'f', 1));
            }
            tableWidget->setItem(i-start+1,j,table_ptr);
            table_ptr->setFont(cell);
        }
    }
    tableWidget->setVerticalHeaderLabels(table_index);

}
void MainWindow::Update_tab_summary(int start, int end, double new_payment[], double new_interest[], double new_principal[], double new_prepay_payment[], double new_prepay_principal[], double new_prepay_interest[])
{
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
    int duration = end - start + 1;
    QStringList table_header;
    table_header<<"Principal"<<"Interest"<<"Payment"<<"Principal \n with Prepay"<<"Interest \n with Prepay"<<"Payment \n with Prepay";
    QTableWidgetItem *table_ptr = 0;
    ui->tableWidget_summary->setColumnCount(6);
    //ui->tableWidget3->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget_summary->setHorizontalHeaderLabels(table_header);
    //setup row coount
    ui->tableWidget_summary->setRowCount(duration);
    //assign values
    filling_item_summary(ui->tableWidget_summary, table_ptr, start, end, new_principal, new_interest, new_payment, new_prepay_principal, new_prepay_interest, new_prepay_payment);
}

void MainWindow::update_summary(double sum_interest, double sum_principal, double sum_prepay_interest, double sum_prepay_principal)
{
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
    ui->label_total_payment->setText(QString("$%L1 kNTD").arg((sum_interest + sum_principal)/1000.0, 0 , 'f', 2));
    ui->label_total_interest->setText(QString("$%L1 kNTD").arg(sum_interest/1000.0, 0 , 'f', 2));
    ui->label_Iratio->setText(QString("%1 %").arg((sum_interest/(sum_interest+sum_principal)*100.0), 0 , 'f', 2));
    if(ui->lineEdit_prepay->text().toDouble() > 0.0)
    {
        ui->label_total_prepay_payment->setText(QString("$%L1 kNTD").arg((sum_prepay_interest + sum_prepay_principal)/1000.0, 0 , 'f', 2));
        ui->label_total_prepay_interest->setText(QString("$%L1 kNTD").arg(sum_prepay_interest/1000.0, 0 , 'f', 2));
        ui->label_prepay_Iratio->setText(QString("%1 %").arg((sum_prepay_interest/(sum_prepay_interest+sum_prepay_principal)*100.0), 0 , 'f', 2));
        ui->label_interest_saved->setText(QString("$%L1 kNTD").arg((sum_interest - sum_prepay_interest)/1000.0, 0 , 'f', 2));
    }
}
void MainWindow::update_monthly(double balance, double sum_interest, double sum_principal, double prepay_balance, double sum_prepay_interest, double sum_prepay_principal)
{
    ui->label_realtime_balance->setText(QString("$%L1 kNTD").arg(balance/1000.0, 0 , 'f', 2));
    ui->label_realtime_interest->setText(QString("$%L1 kNTD").arg(sum_interest/1000.0, 0 , 'f', 2));
    ui->label_realtime_principal->setText(QString("$%L1 kNTD").arg(sum_principal/1000.0, 0 , 'f', 2));
    ui->label_realtime_prepay_balance->setText(QString("$%L1 kNTD").arg(prepay_balance/1000.0, 0 , 'f', 2));
    ui->label_realtime_prepay_interest->setText(QString("$%L1 kNTD").arg(sum_prepay_interest/1000.0, 0 , 'f', 2));
    ui->label_realtime_prepay_principal->setText(QString("$%L1 kNTD").arg(sum_prepay_principal/1000.0, 0 , 'f', 2));
}
void MainWindow::on_pushButton_Calculate_clicked()
{
    clear_alltabs();
    // getting inputs
    Loaninput loan1;
    double precentage = ui->lineEdit_precentage->text().toDouble()/100.0;
    if(ui->lineEdit_precentage->text() == "") {precentage = 0.0;}
    loan1.loan = ui->lineEdit_loan->text().toDouble()*(1.0 - precentage);
    Get(ui->lineEdit_maturity, loan1.maturity);
    loan1.term = loan1.maturity * 12.0;
    printf("maturity:%f\n", loan1.maturity);
    loan1.rate1 = inputget(ui->lineEdit_rate1, loan1.rate1);
    if(ui->lineEdit_rate2->text() == ""){loan1.rate2 = loan1.rate1;}
    else                                {loan1.rate2 = ui->lineEdit_rate2->text().toDouble();}
    if(ui->lineEdit_rate3->text() == ""){loan1.rate3 = loan1.rate2;}
    else                                {loan1.rate3 = ui->lineEdit_rate3->text().toDouble();}
    if(ui->lineEdit_period2->text() == ""){loan1.period2 = loan1.term;}
    else                                  {Get(ui->lineEdit_period2, loan1.period2);}
    if(ui->lineEdit_period3->text() == ""){loan1.period3 = loan1.term;}
    else                                  {Get(ui->lineEdit_period3, loan1.period3);}
    if(ui->lineEdit_grace->text() == ""){loan1.grace = 0.0;}
    else                                  {Get(ui->lineEdit_grace, loan1.grace);}
    loan1.grace = loan1.grace * 12.0;
    if(ui->lineEdit_prepay->text() == ""){loan1.prepay = 0.0;}
    else                                  {loan1.prepay = ui->lineEdit_prepay->text().toDouble();}
    if(ui->lineEdit_prepay_start->text() == ""){loan1.prepay_start = 0.0;}
    else                                  {Get(ui->lineEdit_prepay_start, loan1.prepay_start);}
    if(ui->lineEdit_prepay_end->text() == ""){loan1.prepay_end = 0.0;}
    else                                  {Get(ui->lineEdit_prepay_end, loan1.prepay_end);}
    printf("loan:%f, maturity:%f,rate1:%f,rate2:%f,rate3:%f, period2:%f, period3:%f\n", loan1.loan, loan1.maturity, loan1.rate1, loan1.rate2, loan1.rate3, loan1.period2, loan1.period3);

    //check inputs
    if(loan_check(loan1.loan) ==false ||
       Get(ui->lineEdit_maturity, loan1.maturity) == false ||
       inputget(ui->lineEdit_rate1, loan1.rate1) == 0.0||
       loan1.rate2 ==0 ||
       loan1.rate3 ==0 ||
       grace_check(loan1.term, loan1.grace) == false ||
       period_check(loan1.term, loan1.period2, loan1.period3) == false ||
       prepay_check(loan1.term, loan1.grace, ui->lineEdit_prepay_start, ui->lineEdit_prepay_end) == false)
    {
        ui->glwidget->error_switch = 1;
        ui->glwidget->paint_switch = 2;
        ui->glwidget->paintGL();
    }

    // calculating
    else
    {
        ui->glwidget->paint_switch = 1;
        ui->glwidget->error_switch = 0;
        payment sample1(loan1.loan, loan1.maturity, loan1.grace, loan1.rate1, loan1.rate2, loan1.rate3, loan1.period2, loan1.period3, loan1.prepay, loan1.prepay_start, loan1.prepay_end); //input format:(loan, maturity,rate1,rate2,rate3, period2, period3)
        sample1.calculate_wholterm();
        sample1.period_conversion(12, sample1.interest, sample1.principal, sample1.new_interest, sample1.new_principal, sample1.new_payment);
        sample1.period_conversion(12, sample1.prepay_interest, sample1.prepay_principal, sample1.new_prepay_interest, sample1.new_prepay_principal, sample1.new_prepay_payment);

        //passing values to glwidget class
        ui->glwidget->setArray(loan1.term, sample1.principal, sample1.interest, sample1.PMT, sample1.balance, sample1.sum_interest, sample1.sum_principal, sample1.prepay_balance, sample1.prepay_interest, sample1.prepay_principal, sample1.sum_prepay_interest, sample1.sum_prepay_principal, sample1.prepay_PMT, sample1.new_principal, sample1.new_interest, sample1.new_payment, sample1.new_prepay_principal, sample1.new_prepay_interest, sample1.new_prepay_payment);
        ui->glwidget->setLoaninput(loan1);

        //update tab widgets with result
        if(sample1.pay_period2 != sample1.term) {Update_tab_period(1, 1, sample1.pay_period2-1, sample1.sum_period1, sample1.interest, sample1.principal, sample1.PMT, sample1.rate1, sample1.prepay_principal, sample1.prepay_interest, sample1.prepay_PMT, sample1.sum_prepay_period1);}
        else                                    {Update_tab_period(1, 1, sample1.term, sample1.sum_period1, sample1.interest, sample1.principal, sample1.PMT, sample1.rate1, sample1.prepay_principal, sample1.prepay_interest, sample1.prepay_PMT, sample1.sum_prepay_period1);}
        if(sample1.pay_period2 != sample1.term)
        {
            if(sample1.pay_period3 == sample1.term)
            {
                Update_tab_period(2, sample1.pay_period2, sample1.term, sample1.sum_period2, sample1.interest, sample1.principal, sample1.PMT, sample1.rate2, sample1.prepay_principal, sample1.prepay_interest, sample1.prepay_PMT, sample1.sum_prepay_period2);
            }
            else
            {
                Update_tab_period(2, sample1.pay_period2, sample1.pay_period3 - 1, sample1.sum_period2, sample1.interest, sample1.principal, sample1.PMT, sample1.rate2, sample1.prepay_principal, sample1.prepay_interest, sample1.prepay_PMT, sample1.sum_prepay_period2);
            }
        }
        if(sample1.pay_period3 != sample1.term) {Update_tab_period(3, sample1.pay_period3, sample1.term, sample1.sum_period3, sample1.interest, sample1.principal, sample1.PMT, sample1.rate3, sample1.prepay_principal, sample1.prepay_interest, sample1.prepay_PMT, sample1.sum_prepay_period3);}
        ui->glwidget->paintGL();
        update_summary(sample1.sum_interest[sample1.term - 1], sample1.sum_principal[sample1.term - 1], sample1.sum_prepay_interest[sample1.term-1], sample1.sum_prepay_principal[sample1.term - 1]);
        Update_tab_summary(1, sample1.maturity, ui->glwidget->new_payment, ui->glwidget->new_interest, ui->glwidget->new_principal, ui->glwidget->new_prepay_payment, ui->glwidget->new_prepay_principal, ui->glwidget->new_prepay_interest);
    }
}

void MainWindow::on_pushButton_Quit_clicked()
{
    QCoreApplication::quit();
}
void MainWindow::on_comboBox_graphic_activated(int index)
{
    ui->glwidget->graphic_type = index;
    //printf("graphic index:%d\n", ui->glwidget->graphic_type);
    ui->glwidget->paintGL();
}
void MainWindow::on_tableWidget1_cellClicked(int row, int column)
{
    update_monthly(ui->glwidget->balance[row + 1], ui->glwidget->sum_interest[row], ui->glwidget->sum_principal[row], ui->glwidget->prepay_balance[row + 1], ui->glwidget->sum_prepay_interest[row], ui->glwidget->sum_prepay_principal[row]);
    ui->glwidget->row_highlight = row;
    ui->glwidget->paintGL();
}
void MainWindow::on_tableWidget2_cellClicked(int row, int column)
{
    ui->glwidget->row_highlight = row + ui->lineEdit_period2->text().toInt() - 1;
    int new_row = row + ui->lineEdit_period2->text().toInt() - 1;
    update_monthly(ui->glwidget->balance[new_row + 1], ui->glwidget->sum_interest[new_row], ui->glwidget->sum_principal[new_row], ui->glwidget->prepay_balance[new_row + 1], ui->glwidget->sum_prepay_interest[new_row], ui->glwidget->sum_prepay_principal[new_row]);
    ui->glwidget->paintGL();
}
void MainWindow::on_tableWidget3_cellClicked(int row, int column)
{
    int new_row = row + ui->lineEdit_period3->text().toInt() - 1;
    update_monthly(ui->glwidget->balance[new_row + 1], ui->glwidget->sum_interest[new_row], ui->glwidget->sum_principal[new_row], ui->glwidget->prepay_balance[new_row + 1], ui->glwidget->sum_prepay_interest[new_row], ui->glwidget->sum_prepay_principal[new_row]);
    ui->glwidget->row_highlight = row + ui->lineEdit_period3->text().toInt() - 1;
    ui->glwidget->paintGL();
}
