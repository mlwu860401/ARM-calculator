#ifndef BACKEND_H
#define BACKEND_H

#endif // BACKEND_H
#include "glwidget.h"


class calculation
{
friend class GLwidget;
public:
    double loan;
    double *balance;
    double *interest;
    double *PMT;
    double *principal;
    double *sum_interest;
    double *sum_principal;
    int maturity;
    int term;
    int grace;
    double rate1, rate2, rate3;
    int pay_period2, pay_period3;

    double *prepay_balance;
    double *prepay_interest;
    double *prepay_principal;
    double *sum_prepay_interest;
    double *sum_prepay_principal;
    double *prepay_PMT;
    double prepay;
    double prepay_start;
    double prepay_end;

    double* new_principal;
    double* new_interest;
    double* new_prepay_principal;
    double* new_prepay_interest;
    double* new_payment;
    double* new_prepay_payment;



    calculation(void);
    calculation(double loan, int maturity, int grace, double rate1, double rate2, double rate3, int pay_period2, int pay_period3, double prepay, double prepay_start, double prepay_end);
    ~calculation(void);
};

class payment :public calculation
{
friend class GLwidget;

public:
    double sum_period1 = 0.0;
    double sum_period2 = 0.0;
    double sum_period3 = 0.0;
    double sum_prepay_period1 = 0.0;
    double sum_prepay_period2 = 0.0;
    double sum_prepay_period3 = 0.0;
    int prepay_count = 0;
    payment();
    payment(double loan, int maturity, int grace, double rate1, double rate2, double rate3, int pay_period2, int pay_period3, double prepay, double prepay_start, double prepay_end);
    ~payment();
    double PVAIF(int term, double rate);
    double CAM(double balance, int term, double rate);
    void period_conversion(int new_period, double interest[], double principal[], double new_interest[], double new_principal[], double new_payment[]);
    void calculate_wholterm(void);
};
