#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QObject>
#include <QGLWidget>
#include <QApplication>


struct Loaninput
{
    double loan = {};
    double maturity = {};
    double term = {};
    double grace = {};
    double rate1 = {};
    double rate2 = {};
    double rate3 = {};
    double period2 = {};
    double period3 = {};
    double prepay = {};
    double prepay_start = {};
    double prepay_end = {};

    bool operator==(const Loaninput & o) const
    {
        return  qFuzzyCompare(o.loan, loan) &&
                qFuzzyCompare(o.maturity, maturity) &&
                qFuzzyCompare(o.term, term) &&
                qFuzzyCompare(o.grace, grace) &&
                qFuzzyCompare(o.rate1, rate1) &&
                qFuzzyCompare(o.rate2, rate2) &&
                qFuzzyCompare(o.rate3, rate3) &&
                qFuzzyCompare(o.period2, period2) &&
                qFuzzyCompare(o.period2, period3) &&
                qFuzzyCompare(o.prepay, prepay) &&
                qFuzzyCompare(o.prepay_start, prepay_start) &&
                qFuzzyCompare(o.prepay_end, prepay_end);
    }
};
Q_DECLARE_METATYPE(Loaninput)

class GLwidget : public QGLWidget
{
    Q_OBJECT
    //Q_PROPERTY(double loan READ loan WRITE setLoan)
    Q_PROPERTY(Loaninput loaninput READ loaninput WRITE setLoaninput)

    //double myloan;

    Loaninput my_loan;
    friend class figure;

public:

    explicit GLwidget(QWidget *parent = 0);
    int paint_switch;
    int graphic_type = 0;
    int last = 0;
    int error_switch = 0;
    int error_record = 0;
    int row_highlight = -1;
    int row_last = -1;
    double *principal;
    double *interest;
    double *PMT;
    double *balance;
    double *sum_interest;
    double *sum_principal;
    double *prepay_balance;
    double *prepay_interest;
    double *prepay_principal;
    double *sum_prepay_interest;
    double *sum_prepay_principal;
    double *prepay_PMT;
    double* new_principal;
    double* new_interest;
    double* new_prepay_principal;
    double* new_prepay_interest;
    double* new_payment;
    double* new_prepay_payment;

    int widget_width;
    int widget_height;

    ~GLwidget();
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void valuecheck();
    double digit_component(double incoming);
    double label_value(double input);
    void putlabel(double x_origin, double y_origin, double height_figure, double width_figure, double maturity, double input, char unit[]);
    void draw_legend();
    void period_conversion(int new_period, double interest[], double principal[], double new_interest[], double new_principal[], double new_payment[]);
    void show_valueset(double bar_width, double y_origin, double height_ratio);
    void draw_stackchart(double width_figure, double height_figure);
    void draw_linechart(double width_figure, double height_figure);
    void setArray(int length, double* principal_ptr, double* interest_ptr, double* PMT_ptr, double* balance_ptr, double* sum_interest_ptr, double* sum_principal_ptr, double* prepay_balance_ptr, double* prepay_interest_ptr, double* prepay_principal_ptr, double* sum_prepay_interest_ptr, double* sum_prepay_principal_ptr, double* sum_prepay_PMT_ptr, double* new_principal_ptr, double* new_interest_ptr, double* new_payment_ptr, double* new_prepay_principal_ptr, double* new_prepay_interest_ptr, double* new_prepay_payment_ptr)
    {
        printf("length:%d\n",length);
        principal = new double[length];
        interest = new double[length];
        sum_principal = new double[length];
        sum_interest = new double[length];
        PMT = new double[length];
        balance = new double[length + 1];
        balance[0] = balance_ptr[0];

        prepay_balance = new double [length + 1];
        prepay_balance[0] = prepay_balance_ptr[0];
        prepay_interest = new double[length];
        prepay_principal = new double[length];
        sum_prepay_interest = new double[length];
        sum_prepay_principal = new double[length];
        prepay_PMT = new double[length];

        int maturity = length/12;
        new_principal = new double[maturity];
        new_interest = new double[maturity];
        new_prepay_principal = new double[maturity];
        new_prepay_interest = new double[maturity];
        new_payment = new double[maturity];
        new_prepay_payment = new double[maturity];
        //printf("balance0:%f\n",balance_ptr[0]);
        for(int i = 0; i < length; i++)
        {
            principal[i] = principal_ptr[i];
            interest[i] = interest_ptr[i];
            sum_principal[i] = sum_principal_ptr[i];
            sum_interest[i] = sum_interest_ptr[i];
            PMT[i] = PMT_ptr[i];
            balance[i+1] = balance_ptr[i+1];
            prepay_balance[i+1] = prepay_balance_ptr[i+1];
            prepay_interest[i] = prepay_interest_ptr[i];
            prepay_principal[i] = prepay_principal_ptr[i];
            sum_prepay_interest[i] = sum_prepay_interest_ptr[i];
            sum_prepay_principal[i] = sum_prepay_principal_ptr[i];
            prepay_PMT[i] = sum_prepay_PMT_ptr[i];
        }
        for(int i = 0; i< maturity; i++)
        {
            new_payment[i] = new_payment_ptr[i];
            new_interest[i] = new_interest_ptr[i];
            new_principal[i] = new_principal_ptr[i];
            new_prepay_payment[i] = new_prepay_payment_ptr[i];
            new_prepay_interest[i] = new_prepay_interest_ptr[i];
            new_prepay_principal[i] = new_prepay_principal_ptr[i];
        }
    }
    const Loaninput & loaninput() const { return my_loan; }
    Loaninput getLoan() const { return my_loan; }
    void setLoaninput(const Loaninput & loaninput)
    {
         if (loaninput == my_loan) return;
         my_loan = loaninput;
         update();
    }

private:

};

class figure
{
public:
    int win_height;
    int win_width;

    figure(int win_height, int win_width);
    figure();

    double find_max(int term, double input[]);
    double width_bar(int term);
    double height_bar(double input);
    void draw_figure_lines(double x1, double y1, double x2, double y2, int red, int green, int blue);
    void draw_linechart(double target[], int i, double x_origin, double y_origin, double height_ratio, double width, int red, int green, int blue, int row_highlight, int hollow);
    void draw_PMT(double interest[], double PMT[], int term, double width, double PMT_ratio);
    void draw_stackbar(double interest[], double payment[], int term, double interval, double width, double height_ratio, double x_origin, double y_origin);
    void draw_frame(double x, double y, double x_length, double y_length);
    inline void draw_bar(double x, double y_origin, double length, double width, int fill);
    void draw_rect(double x_origin, double y_origin, double x_length, double y_length, int red, int green, int blue);
    void draw_circle(double x,double y, double daimeter, int hollow);
};

#endif // GLWIDGET_H
