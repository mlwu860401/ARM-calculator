#include <stdio.h>
#include <math.h>
#include "backend.h"
#include <gl/GL.h>

calculation::calculation(double loan, const int maturity, const int grace, double rate1, double rate2, double rate3, int pay_period2, int pay_period3, double prepay, double prepay_start, double prepay_end) :
    loan(loan),
    maturity(maturity),
    term(maturity * 12),
    grace(grace),
    rate1(rate1),
    rate2(rate2),
    rate3(rate3),
    pay_period2(pay_period2),
    pay_period3(pay_period3),
    balance(new double[maturity * 12 + 1]),
    interest(new double[maturity * 12]),
    PMT(new double[maturity * 12]),
    principal(new double[maturity * 12]),
    sum_interest(new double[maturity * 12]),
    sum_principal(new double[maturity * 12]),
    prepay(prepay),
    prepay_start(prepay_start),
    prepay_end(prepay_end),
    prepay_balance(new double[maturity * 12 + 1]),
    prepay_interest(new double[maturity * 12]),
    prepay_principal(new double[maturity * 12]),
    sum_prepay_interest(new double[maturity * 12]),
    sum_prepay_principal(new double[maturity * 12]),
    prepay_PMT(new double[maturity * 12]),
    new_interest(new double[maturity]),
    new_principal(new double[maturity]),
    new_payment(new double[maturity]),
    new_prepay_interest(new double[maturity]),
    new_prepay_principal(new double[maturity]),
    new_prepay_payment(new double[maturity])
{
}
calculation::~calculation(void)
{
    if (balance != nullptr &&
        interest != nullptr &&
        PMT != nullptr &&
        principal != nullptr &&
        sum_interest != nullptr &&
        sum_principal != nullptr &&
        prepay_balance != nullptr &&
        prepay_interest != nullptr &&
        prepay_PMT != nullptr &&
        prepay_principal != nullptr &&
        sum_prepay_interest != nullptr &&
        sum_prepay_principal != nullptr &&
        new_interest != nullptr &&
        new_principal != nullptr &&
        new_prepay_payment != nullptr &&
        new_prepay_interest != nullptr &&
        new_prepay_principal != nullptr)
    {
        delete[] balance;
        balance = nullptr;

        delete[] interest;
        interest = nullptr;

        delete[] PMT;
        PMT = nullptr;

        delete[] principal;
        principal = nullptr;

        delete[] sum_interest;
        sum_interest = nullptr;

        delete[] sum_principal;
        sum_principal = nullptr;

        delete[] prepay_balance;
        prepay_balance = nullptr;

        delete[] prepay_interest;
        prepay_interest = nullptr;

        delete[] prepay_PMT;
        prepay_PMT = nullptr;

        delete[] prepay_principal;
        prepay_principal = nullptr;

        delete[] sum_prepay_interest;
        sum_prepay_interest = nullptr;

        delete[] sum_prepay_principal;
        sum_prepay_principal = nullptr;

        delete[] new_interest;
        new_interest = nullptr;

        delete[] new_principal;
        new_principal = nullptr;

        delete[] new_payment;
        new_payment = nullptr;

        delete[] new_prepay_interest;
        new_prepay_interest = nullptr;

        delete[] new_prepay_principal;
        new_prepay_principal = nullptr;

        delete[] new_prepay_payment;
        new_prepay_payment = nullptr;
        printf("Pointers in Calculation class Nullified!\n");
    }
    else
    {
        printf("Error: Pointer in Calculation class already nullified or uninitialized!\n");
    }
}

payment::payment();

payment::payment(double loan, int maturity, int grace, double rate1, double rate2, double rate3, int pay_period2, int pay_period3, double prepay, double prepay_start, double prepay_end) : calculation(loan, maturity, grace, rate1, rate2, rate3, pay_period2, pay_period3, prepay, prepay_start, prepay_end)
{
    balance[0] = loan;
    prepay_balance[0] = loan;
    //printf("confirm==> balance at year 0 = %f\n", balance[0]);
}
payment::~payment()
{
//    if (balance != nullptr && interest != nullptr)
//    {
//        delete[] balance;
//        balance = nullptr;

//        delete[] interest;
//        interest = nullptr;

//        delete[] PMT;
//        PMT = nullptr;

//        delete[] principal;
//        principal = nullptr;

//        delete[] sum_interest;
//        principal = nullptr;

//        delete[] sum_principal;
//        principal = nullptr;
//        printf("Pointers in Payment class Nullified!\n");
//    }
//    else
//    {
//        printf("Error: Pointer in Payment class already nullified or uninitialized!\n");
//    }
}

double payment::PVAIF(int term, double rate) //checked
{
    double r = rate / 12.0;
    double x = (pow(1.0 + r, term) - 1.0) / (r*pow(1.0 + r, term));
    return x;
}

double payment::CAM(double balance, int term, double rate)
{
    return balance / PVAIF(term, rate);
}
void payment::period_conversion(int new_period, double interest[], double principal[], double new_interest[], double new_principal[], double new_payment[])
{
    double sum_interest = 0.0;
    double sum_principal = 0.0;
    for(int i = 0; i< term; i++)
    {
        sum_interest = sum_interest + interest[i];
        sum_principal = sum_principal + principal[i];
        int index = int(floor(i/new_period));
        if(i % new_period == (new_period - 1))
        {
            new_interest[index] = sum_interest;
            new_principal[index] = sum_principal;
            new_payment[index] = sum_interest + sum_principal;
            sum_interest = 0;
            sum_principal = 0;
            printf("CONVERSION index:%d, new interest:%f, new principal:%f, new payment:%f\n", index, new_interest[index], new_principal[index], new_payment[index]);
        }
   }
}
void payment::calculate_wholterm(void)
{
    double outstanding;
    double prepay_outstanding;
    double r;
    int n;
    double sumInterest = 0.0;
    double sumPrincipal = 0.0;
    double prepay_sumInterest =0.0;
    double prepay_sumPrincipal = 0.0;
    for (int i = 1;i <= term;i++)
    {
        //setting rates and term for different period
        if (i == 1)
        {
            r = rate1/100.0;
            n = term;
            outstanding = balance[0];
            prepay_outstanding = prepay_balance[0];
            //printf("in period 1!\n");

        }
        if (i == pay_period2 && pay_period2 != term)
        {
            r = rate2/100.0;
            n = term + 1 - pay_period2;
            outstanding = balance[pay_period2 - 1];
            prepay_outstanding = prepay_balance[pay_period2 - 1];
            //printf("in period 2!\n");
        }
        if (i == pay_period3 && pay_period3 != term)
        {
            r = rate3/100.0;
            n = term + 1 - pay_period3;
            outstanding = balance[pay_period3 - 1];
            prepay_outstanding = prepay_balance[pay_period3 - 1];
            //printf("in period 3!\n");
        }
        if(i == grace + 1)
        {
            n = term - grace;
        }
        PMT[i-1] = CAM(outstanding, n, r);
        prepay_PMT[i-1] = CAM(prepay_outstanding, n, r);
        if(i <= grace)
        {
            PMT[i-1] = balance[i - 1] * r / 12.0;
            prepay_PMT[i-1] = prepay_balance[i - 1] * r / 12.0;
            printf("prepay PMT:%f\n", prepay_PMT[i-1]);
        }
        //calculate total payment in different period
        interest[i - 1] = balance[i - 1] * r / 12.0;
        sumInterest = sumInterest + interest[i - 1];
        sum_interest[i-1] = sumInterest;
        principal[i-1] = PMT[i-1] - interest[i-1];
        sumPrincipal = sumPrincipal + principal[i-1];
        sum_principal[i-1] = sumPrincipal;
        if (i == pay_period2)
        {
            sum_period1 = sum_interest[i-2] + sum_principal[i-2];
            sum_prepay_period1 = sum_prepay_interest[i-2] + sum_prepay_principal[i-2];
        }
        if (i == pay_period3)
        {
            sum_period2 = sum_interest[i-2] + sum_principal[i-2] - sum_period1;
            sum_prepay_period2 = sum_prepay_interest[i-2] + sum_prepay_principal[i-2] - sum_prepay_period1;
        }
        if (i == term)
        {
            sum_period3 = sum_interest[i-1] + sum_principal[i-1] - sum_period1 - sum_period2;
            sum_prepay_period3 = sum_prepay_interest[i-2] + sum_prepay_principal[i-2] - sum_prepay_period2;
        }
        balance[i] = balance[i - 1] - PMT[i - 1] + interest[i - 1];
        printf("term:%d, balance:%f, interest:%f, principal:%f, paid interest:%f, paid principal:%f, payment:%f\n", i, balance[i], interest[i - 1], principal[i-1], sumInterest, sumPrincipal, PMT[i - 1]);
        if(i < prepay_start && prepay != 0)
        {
            prepay_interest[i-1] = interest[i-1];
            prepay_principal[i-1] = principal[i-1];
            prepay_balance[i] = balance[i];
            sum_prepay_interest[i-1] = sum_interest[i-1];
            sum_prepay_principal[i-1] = sum_principal[i-1];
            prepay_sumInterest = sumInterest;
            prepay_sumPrincipal = sumPrincipal;
            prepay_count ++;
            printf("PREPAY term:%d, prepay_balance:%f, prepay_interest:%f, prepay principal:%f, paid interest:%f, paid principal:%f, payment:%f, count:%d\n", i, prepay_balance[i], prepay_interest[i - 1], prepay_principal[i-1], prepay_sumInterest, prepay_sumPrincipal, PMT[i - 1] + prepay, prepay_count);
        }
        if(i >= prepay_start && prepay_balance[i-1] > 0.0)
        {
            if(i > prepay_end) {prepay =0 ;}
            prepay_interest[i - 1] = prepay_balance[i - 1] * r / 12.0;
            prepay_sumInterest = prepay_sumInterest + prepay_interest[i - 1];
            sum_prepay_interest[i-1] = prepay_sumInterest;
            if(prepay + prepay_PMT[i-1] - prepay_interest[i-1] > prepay_balance[i-1]) {prepay = prepay_balance[i-1] - prepay_PMT[i-1] + prepay_interest[i-1];}
            prepay_principal[i-1] = prepay_PMT[i-1] - prepay_interest[i-1] + prepay;
            prepay_sumPrincipal = prepay_sumPrincipal + prepay_principal[i-1];//?????????????
            sum_prepay_principal[i-1] = prepay_sumPrincipal;
            prepay_balance[i] = prepay_balance[i - 1] - prepay_principal[i-1];
            prepay_count ++;
            printf("PREPAY term:%d, prepay_balance:%f, prepay_interest:%f, prepay principal:%f, paid interest:%f, paid principal:%f, payment:%f, count:%d\n", i, prepay_balance[i], prepay_interest[i - 1], prepay_principal[i-1], prepay_sumInterest, prepay_sumPrincipal, PMT[i - 1] + prepay, prepay_count);
        }
        for(int i = prepay_count + 1; i <= term; i++)
        {
            prepay_balance[i] = 0;
            prepay_interest[i - 1] = 0;
            prepay_principal[i - 1] = 0;
            sum_prepay_interest[i - 1] = sum_prepay_interest[prepay_count - 1];
            sum_prepay_principal[i - 1] = prepay_balance[0];

        }
        //printf("index:%d->", i);
        //printf("confirm ==> index:%d, loan:%f, term:%d, rate:%f\n", i, outstanding, n, r);
    }
}

