#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "glwidget.h"
#include "lengend.h"




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
friend class GLwidget;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    template <class T>
    double inputget(QLineEdit *input, T output);
    template <class H>
    bool Get(QLineEdit *input, H &output);
    bool loan_check(double loan);
    bool grace_check(double term, double grace);
    bool period_check(double term, double period2, double period3);
    bool prepay_check(double term, double grace, QLineEdit *prepay_start, QLineEdit *prepay_end);
    //int check_all(double loan, double maturity, double term, double rate1, double rate2, double rate3, double period2, double period3);
    inline void filling_item(QTableWidget *tableWidget, QTableWidgetItem *table_ptr, int start, int end, double principal[], double interest[], double PMT[], double prepay_principal[], double prepay_interest[], double prepay_PMT[]);
    void clear_alltabs();
    void Update_tab_period(int period_switch, int start, int end, double payment, double interest[], double principal[], double PMT[], double rate, double prepay_principal[], double prepay_interest[], double prepay_PMT[], double prepay_payment);
    void filling_item_summary(QTableWidget *tableWidget, QTableWidgetItem *table_ptr, int start, int end, double new_principal[], double new_interest[], double new_payment[], double new_prepay_principal[], double new_prepay_interest[], double new_prepay_payment[]);
    void Update_tab_summary(int start, int end, double new_payment[], double new_interest[], double new_principal[], double new_prepay_payment[], double new_prepay_principal[], double new_prepay_interest[]);
    void update_summary(double sum_interest, double sum_principal, double sum_prepay_interest, double sum_prepay_principal);
    void update_monthly(double balance, double sum_interest, double sum_principal, double prepay_balance, double sum_prepay_interest, double sum_prepay_principal);


private slots:
    void on_pushButton_Calculate_clicked();
    void on_pushButton_Quit_clicked();

    void on_comboBox_graphic_activated(int index);

    void on_tableWidget1_cellClicked(int row, int column);

    void on_tableWidget2_cellClicked(int row, int column);

    void on_tableWidget3_cellClicked(int row, int column);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
