
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <tablewindow.h>
#include <QMainWindow>
#include <QtCharts>
#include <QChartView>
#include <QtCharts/QLineSeries>
#include "tmessage.h"
#include "QVector"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    TMessage message;


private slots:
    void on_Draw_but_3_clicked();

    void on_Clear_But_3_clicked();

    void on_Exit_But_3_clicked();

    void on_Get_Data_but_3_clicked();

    void on_show_table_but_clicked();



private:
    Ui::MainWindow *ui;
    double X_Left = -10;
    double X_Right = 10;
    double Y_Top = 10;
    double Y_Bot = -10;
    double step = 0.0001;
    double temporary = 0.0;
    double* grid = nullptr;
    double* array_of_values = nullptr;
    double RTOL  = 1e-8;
    double start_position_y = 2;
    double start_position_y_sys = 3;

    tablewindow* table_window;

    int choice = 0; //if choice == 0 => we must to solve test task, else if choice == 1 => we will solve default diff equlation, else choice == 3, we will solve system

    QChartView *chartView = new QChartView();
    QLineSeries *series1 = new QLineSeries();
    QLineSeries *series2 = new QLineSeries();
    QChart *chart = new QChart();
    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();

    size_t count_of_iter = 0;
    bool IsTest = true;
    QVector<double> array_of_xi ;
    QVector<double> array_of_vi ;
    QVector<double> array_of_v2i;
    QVector<double> sub_vi_v2i ;
    QVector<double> OLP ;
    QVector<double> hi;
    QVector<double> C1_Vec ;
    QVector<double> C2_Vec ;
    QVector<double> Ui;
    QVector<double> Ui_Vi;
    QVector<QVector<double>> Vec_Of_Vecs;
    size_t size = 0;

    //void Print();
    //void Insert_In_File(double* new_array,double* array_of_values, size_t count, std::fstream& stream);
    std::string Name_Of_File;

    double Eq_F(double x, double y);

    double Eq_F_TrueSol(double x);

    double Sys_F1(double t, double y1, double y2);
    double Sys_F2(double x, double y1, double y2);

    void Solve_Eq();
    void Solve_Sys();

    double RK4(double x, double y,double h);
    double* RK4_Sys(double x, double y1, double y2, double h);
signals:
    void signal(TMessage& message);

};
#endif // MAINWINDOW_H
