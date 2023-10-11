
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QChartView>
#include <QtCharts/QLineSeries>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_Draw_but_3_clicked();

    void on_Clear_But_3_clicked();

    void on_Exit_But_3_clicked();

    void on_Get_Data_but_3_clicked();



private:
    Ui::MainWindow *ui;
    double X_Left = -10;
    double X_Right = 10;
    double Y_Top = 10;
    double Y_Bot = -10;
    double Accur = 0.0001;
    double temporary = 0.0;
    double* grid = nullptr;
    double* array_of_values = nullptr;

    QChartView *chartView = new QChartView();
    QLineSeries *series1 = new QLineSeries();
    QLineSeries *series2 = new QLineSeries();
    QChart *chart = new QChart();
    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();


    size_t size = 0;
    //void Print();
    //void Insert_In_File(double* new_array,double* array_of_values, size_t count, std::fstream& stream);
    std::string Name_Of_File;
    double RK4(double x, double y,double h);
    double function(double x, double y);
    double function_true(double x);



};
#endif // MAINWINDOW_H
