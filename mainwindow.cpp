#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QDebug>
#include <QColor>
#include<omp.h>
#include <fstream>
#include<iomanip>
#include<iostream>
#include <QtCharts>
#include <QChartView>
#include <QtCharts/QLineSeries>
//using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   // QLineSeries *series = nullptr;

}
//using namespace QtCharts;
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Draw_but_3_clicked()
{
    double temporary = 0;
   // QLineSeries *series1 = new QLineSeries();
   // QLineSeries *series2 = new QLineSeries();

    if(size > 0 && grid != nullptr )
    {
        temporary = function_true(grid[0]);
        series1->append(grid[0],temporary);
        for(size_t i = 1 ; i < size; i++ )
        {
            temporary = RK4(grid[i],temporary, Accur);
            series1->append(grid[i],temporary);
            std::cout<<"AAA"<<std::endl;
        }
    }
    else
         throw "something unexist";

    if(size > 0 && grid != nullptr )
        for(size_t i = 0 ; i < size; i++ )
        {

            series2->append(grid[i],function_true(grid[i]));
            std::cout<<"AAA"<<std::endl;
        }
    else
        throw "something unexist";



    //QChartView *chartView = new QChartView();


    ui->horizontalLayout_10->addWidget(chartView);


   // QChart *chart = new QChart();

    chart->legend()->hide();

   // series1->bestFitLineColorChanged(color_rofl);
    //series1->setBrush(Qt :: red);
    QPen pen(QRgb(0xfdb157));
    pen.setWidth(1);
    series1->setPen(pen);
    chart->addSeries(series1);

    QPen pen1(QRgb(0x0000FF));
    pen.setWidth(1);
    series2->setPen(pen1);
    chart->addSeries(series2);




    chart->setTitle("Graphic");


     //QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("x");
        axisX->setLabelFormat("%i");
    axisX->setTickCount(1);
    chart->addAxis(axisX, Qt::AlignBottom);
    series1->attachAxis(axisX);

   // QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("y");
    axisY->setLabelFormat("%g");
    axisY->setTickCount(10);
    chart->addAxis(axisY, Qt::AlignLeft);
    series1->attachAxis(axisY);


    chartView->setChart(chart);
    std::cout<<"DSSDF"<<std::endl;


}


void MainWindow::on_Clear_But_3_clicked()
{

    //chart->close();
   // chart->zoom(3);

}


void MainWindow::on_Exit_But_3_clicked()
{
     this->close();
}


void MainWindow::on_Get_Data_but_3_clicked()
{
     X_Left = ui->X_EDIT_START_3->text().toDouble();
     X_Right = ui->X_EDIT_END_3->text().toDouble();
     Y_Top = ui->Y_EDIT_START_3->text().toDouble();
     Y_Bot = ui->Y_EDIT_END_3->text().toDouble();
     Accur = ui->Acc_Edit_3->text().toDouble();

     std::cout<<"x = ["<<X_Left<<" ; "<<X_Right<<']'<<std::endl;
     std::cout<<"y = ["<<Y_Bot<<" ; "<<Y_Top<<']'<<std::endl;
     std::cout<<"Accurancy = "<<Accur<<std::endl;

     size = static_cast<size_t>((X_Right - X_Left)/Accur)+1;

     grid = new double[size];
     grid[0] = X_Left;

#pragma omp parallel for shared (grid)
     for(size_t i = 0 ; i <size; i++ )
     {
         if( i == 0)
             std::cout<<omp_get_max_threads()<<std::endl;
         grid[i] =  grid[0] + i*Accur;
     }

}

double MainWindow::RK4(double x,double y, double h)
{
     double k1,k2,k3,k4,k;
     double yn = 0;
     double x0 = x;
     double y0 = y;

     k1 = h * (function(x0, y0));
     k2 = h * (function((x0+h/2), (y0+k1/2)));
     k3 = h * (function((x0+h/2), (y0+k2/2)));
     k4 = h * (function((x0+h), (y0+k3)));
     k = (k1+2*k2+2*k3+k4)/6;
     yn = y0 + k;
     return yn;

}

double MainWindow::function(double x, double y)
{
     return(x/(1+x*x));
}

inline double MainWindow::function_true(double x)
{
     return (log(1+x*x)*0.5);
}



