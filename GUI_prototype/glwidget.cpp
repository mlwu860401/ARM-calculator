#include "glwidget.h"
#include <gl/GL.h>
#include "backend.h"
#include <QFont>

figure::figure(int win_height, int win_width):win_height(win_height), win_width(win_width){}
figure::figure(){}
double figure::find_max(int term, double input[])
{
    double max = 0.0;
    for (int i = 0;i <= term;i++)
    {
        if (max < input[i])
        {
            max = input[i];
        }
    }
    return max;
}
double figure::width_bar(int term)
{
    return (double(win_width) - double(win_width)/10.0*3.0) / double(term);
}
double figure::height_bar(double input)
{
    return input / (double(win_height) - double(win_height)/10.0*3.0);
}
void figure::draw_linechart(double target[], int i, double x_origin, double y_origin, double height_ratio, double width, int red, int green, int blue, int row_highlight, int hollow)
{
    printf("painting line...\n");
    //double y_origin = double(win_height)/2.0 - double(win_height)/80.0;
    glColor3ub(red, green, blue);
    glLineWidth(2);
    glBegin(GL_LINE_STRIP);
    for (int n = 0;n < i;n++)
    {
        glVertex2f(double(n)*width + x_origin, y_origin - target[n] / height_ratio/2);
        //printf("index:%d, balance:%f, x:%f,y:%f\n",n, balance[n], double(n)*width + double(win_width)/40, win_height/2 + balance[n] / height_ratio / 2);
    }
    glEnd();
    glLineWidth(1.f);
    if(row_highlight >= 0)
    {
        draw_circle(x_origin + row_highlight * width,  y_origin - target[row_highlight] / height_ratio/2, 10.0, hollow);
    }
    //renderText(double(win_width)/80.0 + row_highlight * width, y_origin - target[row_highlight] / height_ratio/2, 0.0, QString::number(target[row_highlight]));
    printf("painting done\n");
}
void figure::draw_stackbar(double interest[], double payment[], int term, double interval, double width, double height_ratio, double x_origin, double y_origin)
{
    printf("painting ratio chart...\n");
    for (int n = 0; n < term; n++)
    {
        glColor3ub(30,144,255);
        draw_bar(double(n)*interval + x_origin, y_origin, payment[n]/height_ratio/2, width, 1);
        glColor3ub(220,20,60);
        draw_bar(double(n)*interval + x_origin, y_origin, interest[n]/height_ratio/2.0, width, 1);
        if (width >= 5)
        {
            glColor3ub(0, 0, 0);
            draw_bar(double(n)*interval + x_origin, y_origin, payment[n]/height_ratio/2.0, width, 0);
            draw_bar(double(n)*interval + x_origin, y_origin, interest[n]/height_ratio/2.0, width, 0);
        }
    }
    printf("painting done!\n");
}
inline void figure::draw_bar(double x, double y_origin, double length, double width, int fill)
{
    if (fill == 1){ glBegin(GL_QUADS); }
    if (fill == 0){ glBegin(GL_LINE_STRIP); }
    printf("length: %f\n", length);
    glVertex2d(x, y_origin);
    glVertex2d(x + width, y_origin);
    glVertex2d(x + width, y_origin - length);
    glVertex2d(x, y_origin - length);
    glVertex2d(x , y_origin);
    glEnd();
}
void figure::draw_PMT(double interest[], double PMT[], int term, double width, double PMT_height)
{
    printf("painting PMT chart...\n");
    for (int n = 0;n < term; n++)
    {
        glColor3ub(0, 156, 0);
        draw_bar(double(n) * width, double(win_height)*39.0/80.0, PMT[n]/PMT_height/2, width,1);
        glColor3ub(204, 0, 0);
        draw_bar(double(n) * width, double(win_height)*39.0/80.0, interest[n]/PMT_height/2, width,1);
        if (width >= 5)
        {
            glColor3ub(0, 0, 0);
            draw_bar(double(n) * width, double(win_height)*39.0/80.0, PMT[n] / PMT_height/2, width, 0);
            draw_bar(double(n) * width, double(win_height)*39.0/80.0, interest[n] / PMT_height/2, width, 0);
        }
        printf("index:%d, x:%f, y:%f\n", n, double(n) * width, PMT[n] / PMT_height/2);
    }
    printf("paint done\n");
}
void figure::draw_figure_lines(double x1, double y1, double x2, double y2, int red, int green, int blue)
{
    glColor3ub(red, green, blue);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void figure::draw_frame(double x, double y, double x_length, double y_length)
{
    printf("painting frame...\n");
    // x & y axis
    draw_figure_lines(x, y, x -x_length, y, 0, 0, 0);
    draw_figure_lines(x, y, x, y -y_length, 0, 0, 0);

    // indicators
    draw_figure_lines(x, y- y_length, x + 10.0, y - y_length, 0, 0, 0);
    draw_figure_lines(x, y- y_length/2.0, x + 10.0, y - y_length/2.0, 0, 0, 0);
    draw_figure_lines(x, y- y_length/4.0, x + 10.0, y - y_length/4.0, 0, 0, 0);
    draw_figure_lines(x, y- y_length/4.0*3.0, x + 10.0, y - y_length/4.0*3.0, 0, 0, 0);
    draw_figure_lines(x, y, x + 10.0, y, 0, 0, 0);
    draw_figure_lines(x, y, x, y + 10.0, 0, 0, 0);
    draw_figure_lines(x - x_length, y, x - x_length, y + 10.0, 0, 0, 0);

    // reference lines
    draw_figure_lines(x, y- y_length, x - x_length, y - y_length, 220, 220, 220);
    draw_figure_lines(x, y- y_length/2.0, x - x_length, y - y_length/2.0, 220, 220, 220);
    draw_figure_lines(x, y- y_length/4.0, x - x_length, y - y_length/4.0, 220, 220, 220);
    draw_figure_lines(x, y- y_length/4.0*3.0, x - x_length, y - y_length/4.0*3.0, 220, 220, 220);
    printf("painting done\n");
}
void figure::draw_rect(double x_origin, double y_origin, double x_length, double y_length, int red, int green, int blue)
{
    glColor3ub(red, green, blue);
    glBegin(GL_QUADS);
    glVertex2f(x_origin,y_origin);
    glVertex2f(x_origin + x_length, y_origin);
    glVertex2f(x_origin + x_length, y_origin + y_length);
    glVertex2f(x_origin, y_origin + y_length);
    glEnd();
}
void figure::draw_circle(double x,double y, double daimeter, int hollow)
{
    double pi = 3.1416;
    if(hollow == 1) {glBegin(GL_POLYGON);}
    glLineWidth(2);
    if(hollow == 0) {glBegin(GL_LINE_STRIP);}
    for(int i = 0; i<65; i++)
    {
        glVertex2f(x + daimeter*cos(double(i)*pi/32.0), y + daimeter*sin(double(i)*pi/32.0));
    }
    glEnd();
    glLineWidth(1.f);

}

double GLwidget::label_value(double input)
{
    double label_value;
    double value_unit = 5.0 * pow(10.0,digit_component(input)-1);
    if(fmod(input, value_unit) ==0)    {label_value = input;}
    else                              {label_value = (floor(input/value_unit) + 1) * value_unit;}
    return label_value;
}

void GLwidget::putlabel(double x_origin, double y_origin, double height_figure, double width_figure, double term, double input, char unit[])
{
    QFont font1("Times", 10);

    glColor3ub(0,0,0);
    renderText(x_origin + 10.0, y_origin, 0.0, QString("0 ").append(unit), font1);
    renderText(x_origin + 10.0, y_origin + 5.0 - height_figure/4.0, 0.0, QString("%1 ").arg(label_value(input)/4.0).append(unit), font1);
    renderText(x_origin + 10.0, y_origin + 5.0 - height_figure/2.0, 0.0, QString("%1 ").arg(label_value(input)/2.0).append(unit), font1);
    renderText(x_origin + 10.0, y_origin + 5.0 - height_figure/4.0*3.0, 0.0, QString("%1 ").arg(label_value(input)/4.0*3.0).append(unit), font1);
    renderText(x_origin + 10.0, y_origin + 5.0 - height_figure, 0.0, QString("%1 ").arg(label_value(input)).append(unit), font1);   
    renderText(x_origin - 3.0 - width_figure, y_origin + 20.0, 0.0, QString("%1").arg(0));
    if(graphic_type == 0)
    {
        renderText(x_origin - 10.0, y_origin + 20.0, 0.0, QString("%1").arg(term).append("th month"), font1);
    }
    else
    {
        renderText(x_origin - 10.0, y_origin + 20.0, 0.0, QString("%1").arg(my_loan.maturity).append("th year"), font1);
    }

}
void GLwidget::draw_legend()
{
    QFont fig_title("times", 20);
    QFont fig_label("times", 12);
    double legend_width;
    double legend_height;
    figure legend;
    if(graphic_type == 0)
    {
        legend_width = width()/40.0;
        legend_height = width() / 800.0;
        glColor3ub(32,178,170);
        legend.draw_bar(width()/80.0, height()/20.0*3.0, legend_height, legend_width, 1);//balance
        legend.draw_circle(width()/80.0 + legend_width/2.0, height()/20.0*3.0 - 1.0, 10.0, 1);
        glColor3ub(220,20,60);
        legend.draw_bar(width()/80.0, height()/20.0*5.0, legend_height, legend_width, 1);
        legend.draw_circle(width()/80.0 + legend_width/2.0, height()/20.0*5.0 - 1.0, 10.0, 1);
        //draw_bar(x_origin - rect_length, widget_height/20.0*19.0, rect_length, 3.0, 220,20,60);//interest
        glColor3ub(30, 144, 255);
        legend.draw_bar(width()/80.0, height()/20.0*7.0, legend_height, legend_width, 1);//principal
        legend.draw_circle(width()/80.0 + legend_width/2.0, height()/20.0*7.0 - 1.0, 10.0, 1);
        glColor3ub(32,178,170);
        legend.draw_bar(width()/80.0, height()/20.0*3.0 + height()/2.0, legend_height, legend_width, 1);//balance
        legend.draw_circle(width()/80.0 + legend_width/2.0, height()/20.0*3.0 - 1.0 + height()/2.0, 10.0, 0);
        glColor3ub(220,20,60);
        legend.draw_bar(width()/80.0, height()/20.0*5.0 + height()/2.0, legend_height, legend_width, 1);
        legend.draw_circle(width()/80.0 + legend_width/2.0, height()/20.0*5.0 + height()/2.0 - 1.0, 10.0, 0);
        //draw_bar(x_origin - rect_length, widget_height/20.0*19.0, rect_length, 3.0, 220,20,60);//interest
        glColor3ub(30, 144, 255);
        legend.draw_bar(width()/80.0, height()/20.0*7.0 + height()/2.0, legend_height, legend_width, 1);//principal
        legend.draw_circle(width()/80.0 + legend_width/2.0, height()/20.0*7.0 + height()/2.0 - 1.0, 10.0, 0);

        //render text
        glColor3ub(0,0,0);
        renderText(width()/2.0 - 20.0, 35.0, 0.0, "Amortization", fig_title);
        renderText(width()/2.0 - 120.0, height()/2.0 + 15.0, 0.0, "Amortization with Prepayment", fig_title);
        renderText(width()/80.0 + legend_width + 10.0, height()/20.0*3.0 + 5.0, 0.0, "Balance", fig_label);
        renderText(width()/80.0 + legend_width + 10.0, height()/20.0*5.0 + 5.0, 0.0, "Paid Interest", fig_label);
        renderText(width()/80.0 + legend_width + 10.0, height()/20.0*7.0 + 5.0, 0.0, "Paid Principal", fig_label);
        renderText(width()/80.0 + legend_width + 10.0, height()/20.0*3.0 + height()/2.0 + 5.0, 0.0, "Balance", fig_label);
        renderText(width()/80.0 + legend_width + 10.0, height()/20.0*5.0 + height()/2.0 + 5.0, 0.0, "Paid Interest", fig_label);
        renderText(width()/80.0 + legend_width + 10.0, height()/20.0*7.0 + height()/2.0 + 5.0, 0.0, "Paid Principal", fig_label);
    }
    if(graphic_type == 1)
    {
        legend_width = width()/8.0;
        legend_height = width() / 100.0;
        glColor3ub(0,0,0);
        renderText(width()/2.0 - 50.0, 45.0, 0.0, "Ratio Chart", fig_title);
        renderText(width()/2.0 - 150.0, height()/2.0 + 5.0, 0.0, "Ratio Chart with Prepayment", fig_title);
        glColor3ub(220,20,60);
        legend.draw_bar(width()/80.0, height()/3.0*1.0, legend_height, legend_width, 1);
        //draw_bar(x_origin - rect_length, widget_height/20.0*19.0, rect_length, 3.0, 220,20,60);//interest
        glColor3ub(0,0,0);
        renderText(width()/80.0 + legend_width/5.0, height()/3.0*1.0 + 25.0, 0.0, "Interest", fig_label);
        legend.draw_bar(width()/80.0, height()/3.0*1.0, legend_height, legend_width, 0);
        glColor3ub(30, 144, 255);
        legend.draw_bar(width()/80.0, height()/3.0*2.0, legend_height, legend_width, 1);//principal
        glColor3ub(0,0,0);
        renderText(width()/80.0 + legend_width/5.0, height()/3.0*2.0 + 25.0, 0.0, "Principal", fig_label);
        legend.draw_bar(width()/80.0, height()/3.0*2.0, legend_height, legend_width, 0);
    }
}

GLwidget::~GLwidget()
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
        printf("Pointers in GLwidget class Nullified!\n");
    }
    else
    {
        printf("Error: Pointer already nullified or uninitialized!\n");
    }
}
void GLwidget::valuecheck()
{
    printf("loan:%f, term:%f\n", my_loan.loan, my_loan.term);
    for(int i = 0; i < my_loan.term; i++)
    {
        printf("index:%d,principal:%f,interest:%f,PMT:%f,balance:%f\n", i, principal[i], interest[i], PMT[i], balance[i+1]);
    }
}
double GLwidget::digit_component(double incoming)
{
    int digit_count = 0;
    while(incoming >= 10.0)
    {
        incoming = incoming / 10.0;
        digit_count ++;
    }
    return digit_count;
}
void GLwidget::period_conversion(int new_period, double interest[], double principal[], double new_interest[], double new_principal[], double new_payment[])
{
    double sum_interest = 0.0;
    double sum_principal = 0.0;
    for(int i = 0; i< my_loan.term; i++)
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
void GLwidget::show_valueset(double bar_width, double y_origin, double height_ratio)
{
    if(row_highlight >= 0)
    {
        glColor3ub(0,0,0);
        renderText(width()/10.0 + row_highlight * bar_width, y_origin - balance[row_highlight + 1]/height_ratio/2.0, 0.0, QString::number(balance[row_highlight + 1]));
    }
}
void GLwidget::draw_stackchart(double width_figure, double height_figure)
{
    double x_origin_upstackchart = double(width()) - double(width())/10.0 ;
    double y_origin_upstackchart = double(height())/2.0 - double(height())/20.0;
    double x_origin_downstackchart = double(width()) - double(width())/10.0 ;
    double y_origin_downstackchart = double(height())*9.0/10.0;
    char unit_up[] = "kNTD";
    int new_period = 12;
    int new_term = int(my_loan.term)/new_period;

    period_conversion(new_period, interest, principal, new_interest, new_principal, new_payment);
    period_conversion(new_period, prepay_interest, prepay_principal, new_prepay_interest, new_prepay_principal, new_prepay_payment);
    int spacer_count = new_term + 1;
    int stack_count = new_term;
    double stack_width = double((width() - width()/10.0 - width()/5.0)/ (spacer_count * 2 + stack_count));
    double spacer_width = 2.0* stack_width;
    for(int i = 0; i < new_term; i++)
    {
        printf("new payment:%f\n", new_payment[i]);
    }
    figure stackchart(height(), width());
    printf("found max:%f\n", stackchart.find_max(new_term, new_payment));
    //double stack_height_ratio = payment_fig.height_bar(payment_fig.find_max(new_term, new_payment));
    double stack_height_ratio_prepay = stackchart.height_bar(label_value(stackchart.find_max(new_term, new_prepay_payment)));
    stackchart.draw_stackbar(new_interest, new_payment, new_term, stack_width + spacer_width, stack_width, stack_height_ratio_prepay, double(width())/5.0 + spacer_width, y_origin_upstackchart);
    stackchart.draw_frame(x_origin_upstackchart, y_origin_upstackchart, width_figure, height_figure);
    putlabel(x_origin_upstackchart, y_origin_upstackchart, height_figure, width_figure, my_loan.term, stackchart.find_max(new_term, new_prepay_payment)/1000.0, unit_up);
    stackchart.draw_stackbar(new_prepay_interest, new_prepay_payment, new_term, stack_width + spacer_width, stack_width, stack_height_ratio_prepay, double(width())/5.0 + spacer_width, y_origin_downstackchart);
    stackchart.draw_frame(x_origin_downstackchart, y_origin_downstackchart, width_figure, height_figure);
    putlabel(x_origin_downstackchart, y_origin_downstackchart, height_figure, width_figure, my_loan.term, stackchart.find_max(new_term, new_prepay_payment)/1000.0, unit_up);
}
void GLwidget::draw_linechart(double width_figure, double height_figure)
{
    figure linechart(height(),width());
    char unit_up[] = "kNTD";

    double height_bar = (double(height()) - double(height())/10.0*3.0);
    double bar_width = (double(width()) - double(width())/10.0*3.0) / (my_loan.term);

    double sum_interest_height_ratio = label_value(sum_interest[int(my_loan.term)-1])/ height_bar;
    double sum_principal_height_ratio = label_value(my_loan.loan)/ height_bar;
    double sum_prepay_interest_height_ratio = label_value(sum_prepay_interest[int(my_loan.term)-1])/ height_bar;
    double sum_prepay_principal_height_ratio = label_value(my_loan.loan)/ height_bar;

    double x_origin_uplinechart = double(width()) - double(width())/10.0 - bar_width;
    double y_origin_uplinechart = double(height())/2.0 - double(height())/20.0;
    double x_origin_downlinechart = double(width()) - double(width())/10.0 - bar_width;
    double y_origin_downlinechart = double(height())*9.0/10.0;

    if(sum_principal_height_ratio>=sum_interest_height_ratio)
    {
        linechart.draw_linechart(sum_interest, my_loan.term, width()/5.0, y_origin_uplinechart, sum_principal_height_ratio, bar_width, 220, 20, 60, row_highlight,1);//red
        linechart.draw_linechart(sum_principal, my_loan.term, width()/5.0, y_origin_uplinechart, sum_principal_height_ratio, bar_width, 30, 144, 255, row_highlight,1);// blue
        linechart.draw_linechart(balance, my_loan.term, width()/5.0, y_origin_uplinechart, sum_principal_height_ratio, bar_width, 32, 178, 170, row_highlight,1);//green
        //show_valueset(bar_width, y_origin_uplinechart, sum_principal_height_ratio);

        linechart.draw_linechart(sum_prepay_interest, my_loan.term, width()/5.0, y_origin_downlinechart, sum_prepay_principal_height_ratio, bar_width, 220, 20, 60, row_highlight,0);//red
        linechart.draw_linechart(sum_prepay_principal, my_loan.term, width()/5.0, y_origin_downlinechart, sum_prepay_principal_height_ratio, bar_width, 30, 144, 255, row_highlight,0);// blue
        linechart.draw_linechart(prepay_balance, my_loan.term, width()/5.0, y_origin_downlinechart, sum_prepay_principal_height_ratio , bar_width, 32, 178, 170, row_highlight,0);//green
    }
    else
    {
        linechart.draw_linechart(sum_interest, my_loan.term, width()/5.0, y_origin_uplinechart, sum_interest_height_ratio, bar_width, 220, 20, 60, row_highlight,1);//red
        linechart.draw_linechart(sum_principal, my_loan.term, width()/5.0, y_origin_uplinechart, sum_interest_height_ratio, bar_width, 30, 144, 255, row_highlight ,1);// blue
        linechart.draw_linechart(balance, my_loan.term, width()/5.0, y_origin_uplinechart, sum_interest_height_ratio, bar_width, 32, 178, 170, row_highlight, 1);//green

        linechart.draw_linechart(sum_prepay_interest, my_loan.term, width()/5.0, y_origin_downlinechart, sum_prepay_interest_height_ratio, bar_width, 220, 20, 60, row_highlight, 0);//red
        linechart.draw_linechart(sum_prepay_principal, my_loan.term, width()/5.0, y_origin_downlinechart, sum_prepay_interest_height_ratio, bar_width, 30, 144, 255, row_highlight, 0);// blue
        linechart.draw_linechart(prepay_balance, my_loan.term, width()/5.0, y_origin_downlinechart, sum_prepay_interest_height_ratio, bar_width, 32, 178, 170, row_highlight, 0);//green
    }

    if(sum_interest[int(my_loan.term)-1] < balance[0])
    {
        putlabel(x_origin_uplinechart, y_origin_uplinechart, height_figure, width_figure - bar_width, my_loan.term, balance[0]/1000.0, unit_up);
        putlabel(x_origin_downlinechart, y_origin_downlinechart, height_figure, width_figure - bar_width, my_loan.term, prepay_balance[0]/1000.0, unit_up);
    }
    else
    {
        putlabel(x_origin_uplinechart, y_origin_uplinechart, height_figure, width_figure - bar_width, my_loan.term, sum_interest[int(my_loan.term)-1]/1000.0, unit_up);
        putlabel(x_origin_downlinechart, y_origin_downlinechart, height_figure, width_figure - bar_width, my_loan.term, sum_prepay_interest[int(my_loan.term)-1]/1000.0, unit_up);
    }
    linechart.draw_frame(x_origin_uplinechart, y_origin_uplinechart, width_figure - bar_width, height_figure);
    linechart.draw_frame(x_origin_downlinechart, y_origin_downlinechart, width_figure - bar_width, height_figure);
}

GLwidget::GLwidget(QWidget *parent):QGLWidget(parent)
{   
}
void GLwidget::initializeGL()
{
    paint_switch = 0;
    glClearColor(1.0,1.0,1.0,0.2);
}
void GLwidget::paintGL()
{
    printf("row highlight:%d, row last:%d\n", row_highlight, row_last);
    //valuecheck();
    //printf("width:%d, height:%d\n", width(), height());
    //printf("switch:%d\n", paint_switch);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    figure payment_fig(height(), width());

    if( paint_switch == 1)
    {
        draw_legend();
        if(graphic_type != last)
        {
            update();
        }
        last = graphic_type;
        if(error_switch != error_record)
        {
            update();
        }
        error_record = error_switch;
        if(row_highlight != row_last)
        {
            update();
        }
        row_last = row_highlight;
        printf("painting figures...\n");
        double width_figure = double(payment_fig.win_width) - double(payment_fig.win_width)/10.0*3.0;
        double height_figure = double(payment_fig.win_height)*7.0/20.0;

        // start drawing
        if(graphic_type == 1)
        {
            draw_stackchart(width_figure, height_figure);
        }
        if(graphic_type == 0)
        {
            draw_linechart(width_figure,height_figure);
        }
        printf("painting complete!\n");
    }
    if(paint_switch == 2)
    {
        if(error_switch != error_record)
        {
            update();
        }
        error_record = error_switch;
        QFont error("times", 30);
        glColor3ub(255,0,0);
        renderText(width()/2.0,height()/2.0,0.0, "Invalid Input(s)", error);
    }
}
void GLwidget::resizeGL(int width, int height)
{
    //printf("width:%d, height:%d\n", width, height);
    widget_width = width;
    widget_height = height;
    glViewport(0,0,width,height);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0,width, height, 0, 0, 1) ;
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
}
