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
#include "tmessage.h"

#define METHOD_ORDER 4
//#define RTOL 1e-8
#define MINSTEP 1e-8
#define A 0.01
#define B 5

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);






}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::Solve_Eq(){
    unsigned int meth_char = pow(2, METHOD_ORDER);
    size_t C1= 0; size_t C2 = 0;
    double tstep = step;
    double local_error = 0;
    double local_error_abs = 0;
    double xcurr = X_Left;
    //double ycurr = Eq_F_TrueSol(X_Left);
    double ycurr = start_position_y;
    double yn = 0;
    double yt = 0;

    series1->append(xcurr, ycurr);

    array_of_xi.push_back(xcurr);
    array_of_vi.push_back(ycurr);
    array_of_v2i.push_back(ycurr);
    sub_vi_v2i.push_back(0);
    OLP.push_back(0);
    hi.push_back(tstep);
    C1_Vec.push_back(0);
    C2_Vec.push_back(0);
    while(xcurr < X_Right)
    {

        yn = RK4(xcurr, ycurr, tstep);
        yt = RK4(xcurr + tstep/2, RK4(xcurr, ycurr, tstep/2), tstep/2);
        local_error= ((yt - yn) / (meth_char - 1)) * (meth_char);
        local_error_abs = abs(local_error);
        hi.push_back(tstep);


        if((local_error_abs >= (RTOL / (meth_char*2)))&&(local_error_abs <= RTOL)){
            ycurr = yn;
            xcurr += tstep;
        } else if(local_error_abs < (RTOL / (meth_char*2))){
            ycurr = yn;
            xcurr += tstep;
            tstep = tstep * 2;
            C2++;
        } else if(local_error_abs > RTOL){
            tstep = fmax(MINSTEP, tstep / 2);
            ycurr = RK4(xcurr, ycurr, tstep);
            xcurr += tstep;
            C1++;
        }
        C1_Vec.push_back(C1);
        C2_Vec.push_back(C2);
        std::cout<<tstep<<' '<<xcurr<<' '<<ycurr<<' '<<local_error_abs<<' '<<RTOL<<' '<<(RTOL / (meth_char*2))<<' '<<(local_error_abs < (RTOL / (meth_char*2)))<<' '<<(local_error_abs > RTOL)<<std::endl;

        series1->append(xcurr,ycurr);


        array_of_xi.push_back(xcurr);
        array_of_vi.push_back(ycurr);
        array_of_v2i.push_back(yt);
        sub_vi_v2i.push_back(ycurr-yt);
        OLP.push_back(local_error_abs);
        count_of_iter++;



        //std::cout<<tstep<<' '<<xcurr<<' '<<ycurr<<' '<<std::endl;
    }

    Vec_Of_Vecs.push_back(array_of_xi);
    Vec_Of_Vecs.push_back(array_of_vi);
    Vec_Of_Vecs.push_back(array_of_v2i);
    Vec_Of_Vecs.push_back(sub_vi_v2i);
    Vec_Of_Vecs.push_back(OLP);
    Vec_Of_Vecs.push_back(hi);
    Vec_Of_Vecs.push_back(C1_Vec);
    Vec_Of_Vecs.push_back(C2_Vec);




  //  for(size_t i = 0; i < array_of_xi.size();i++)
  //  {
        //std::cout<<i<<' '<<hi[i]<<' '<<array_of_xi[i]<<' '<<array_of_vi[i]<<' '<<OLP[i]<<std::endl;
  //  }

    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setRowCount(count_of_iter+1);



    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"X_i"<<"V_i"<<"V2i"<<"V_i - V2i"<<"OLP"<<"h_i"<<"C1"<<"C2");
    for(size_t i = 0 ; i < ui->tableWidget->columnCount();i++)
    {
        for(size_t j = 0 ; j < ui->tableWidget->rowCount();j++)
        {
            QTableWidgetItem *tbl = new QTableWidgetItem(QString::number(Vec_Of_Vecs[i][j]));

            ui->tableWidget->setItem(j,i,tbl);
            std::cout<<i<<std::endl;


        }
    }
}

void MainWindow::Solve_Sys(){
    unsigned int meth_char = pow(2, METHOD_ORDER);
    double tstep = step;
    double local_error = 0;
    double xcurr = X_Left;
    double y1curr = start_position_y;
    double y2curr = start_position_y_sys;
    double* ynn = new double[2];
    double* ynt1 = new double[2];
    double* ynt2 = new double[2];
    double local_error_abs = 0;

    // Usse step = 0.0001 and x = [0, 0.001]

    while((xcurr + tstep) < X_Right)
    {
        ynt1 = RK4_Sys(xcurr, y1curr, y2curr, tstep/2);
        ynt2 = RK4_Sys(xcurr + tstep/2, ynt1[0], ynt1[1], tstep/2);
        ynn = RK4_Sys(xcurr,  y1curr,  y2curr,  tstep);
        local_error= sqrt(pow((ynt2[0] - ynn[0]) / (meth_char - 1), 2) + pow((ynt2[1] - ynn[1]) / (meth_char - 1), 2));
        local_error_abs = fabs(local_error);
        if((local_error_abs >= (RTOL / (meth_char*2)))&&(local_error_abs <= RTOL)){
            y1curr = ynn[0];
            y2curr = ynn[1];
            xcurr += tstep;
        } else if(local_error_abs<(RTOL / (meth_char*2))){
            y1curr = ynn[0];
            y2curr = ynn[1];
            xcurr += tstep;
            tstep = tstep * 2;
        } else if(local_error_abs>RTOL){
            tstep = fmax(MINSTEP, tstep / 2);
            ynn = RK4_Sys(xcurr,  y1curr,  y2curr,  tstep);
            y1curr = ynn[0];
            y2curr = ynn[1];
            xcurr += tstep;
        }
        if(fabs(y1curr) > fabs(1e+10) || fabs(y2curr)>1e+10) break;

        //std::cout<<tstep<<' '<<xcurr<<' '<<y1curr<<' '<<y2curr<<' '<<local_error_abs<<' '<<RTOL<<' '<<(RTOL / (meth_char*2))<<' '<<(local_error_abs < (RTOL / (meth_char*2)))<<' '<<(local_error_abs > RTOL)<<std::endl;

        series1->append(xcurr, y1curr);




    }

    delete[] ynn;
    delete[] ynt1;
    delete[] ynt2;

}

void MainWindow::on_Draw_but_3_clicked()
{

    if(choice == 0)
    {
        temporary = start_position_y;
        for(size_t i = 0 ; i < size; i++ )
        {

            series2->append(X_Left+i*step,Eq_F_TrueSol(X_Left+i*step));

            temporary = RK4(X_Left+i*step,temporary,step);
            series1->append(X_Left+i*step,temporary);

        }
       // std::cout<<series1<<std::endl;
       // std::cout<<std::endl;
       // std::cout<<series2<<std::endl;

        ui->horizontalLayout_10->addWidget(chartView);
        chart->legend()->hide();

        QPen pen(QRgb(0xfdb157));
        pen.setWidth(1);
        series1->setPen(pen);
        chart->addSeries(series1);

        QPen pen1(QRgb(0x0000FF));
        pen.setWidth(1);
        series2->setPen(pen1);
        chart->addSeries(series2);

        //std::cout<<"count = "<<series2->count()<<std::endl;
    }

    if(choice == 1)
    {
        ui->horizontalLayout_10->addWidget(chartView);
        chart->legend()->hide();

        Solve_Eq();
        QPen pen(QRgb(0xfdb157));
        pen.setWidth(1);
        series1->setPen(pen);
        chart->addSeries(series1);
       // std::cout<<"count = "<<series1->count()<<std::endl;
    }

    if(choice == 2)
    {
        ui->horizontalLayout_10->addWidget(chartView);
        chart->legend()->hide();
        Solve_Sys();

        QPen pen(QRgb(0xfdb157));
        pen.setWidth(1);
        series1->setPen(pen);
        chart->addSeries(series1);
       // std::cout<<"count = "<<series1->count()<<std::endl;
    }








    chart->setTitle("Graphic");
    //QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("x");
    axisX->setLabelFormat("%f");
    axisX->setTickCount(1);
    chart->addAxis(axisX, Qt::AlignBottom);
    series1->attachAxis(axisX);


    // QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("y");
    axisY->setLabelFormat("%f");
    axisY->setTickCount(10);
    chart->addAxis(axisY, Qt::AlignLeft);
    series1->attachAxis(axisY);
    chartView->setChart(chart);
    // std::cout<<"DSSDF"<<std::endl;
}


void MainWindow::on_Clear_But_3_clicked(){

    series1->detachAxis(axisX);
    series1->detachAxis(axisY);
    series2->detachAxis(axisX);
    series2->detachAxis(axisY);

     std::cout<<"a"<<std::endl;
    chart->removeAxis(axisX);
    chart->removeAxis(axisY);
     std::cout<<"b"<<std::endl;
    ui->horizontalLayout_10->removeWidget(chartView);
     std::cout<<"qqq"<<std::endl;
    chart->removeSeries(series1);
    chart->removeSeries(series2);
     std::cout<<"c"<<std::endl;
    disconnect(chartView);
    disconnect(chart);
     std::cout<<"d"<<std::endl;
    delete chart;
    delete chartView;
    delete axisX;
    delete axisY;

    if(series1->count() != 0) series1->clear();
    if(series2->count() != 0) series2->clear();

    chartView = new QChartView();
    chart = new QChart();
    axisX = new QValueAxis();
    axisY = new QValueAxis();


    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    //ui->horizontalLayout_10->removeWidget(chartView)

    Vec_Of_Vecs.clear();

    array_of_xi.clear();
    array_of_vi.clear();
    array_of_v2i.clear();
    sub_vi_v2i.clear();
    OLP.clear();
    hi.clear();
    C1_Vec.clear();
    C2_Vec.clear();



    count_of_iter = 0;


}


void MainWindow::on_Exit_But_3_clicked(){
    exit(0);

}

void MainWindow::on_show_table_but_clicked()
{


}
void MainWindow::on_Get_Data_but_3_clicked()
{
    X_Left = ui->X_EDIT_START_3->text().toDouble();
    X_Right = ui->X_EDIT_END_3->text().toDouble();

    RTOL = ui->Acc_Edit_3->text().toDouble();
    step = ui->StepEdit_3->text().toDouble();
    start_position_y = ui->Start_Posit->text().toDouble();
    start_position_y_sys = ui->Start_y_for_sys->text().toDouble();

    if(ui->Test_task_button->isChecked())
    {
        choice = 0;
    }
    else if(ui->main_task->isChecked())
    {
        choice = 1;
    }
    else if(ui->main_task_syst->isChecked())
    {
        choice = 2;
    }
    else
        this->close();

    size = static_cast<size_t>((X_Right - X_Left)/step)+1;

    grid = new double[size];
    grid[0] = X_Left;

#pragma omp parallel for shared (grid)
    for(size_t i = 0 ; i <size; i++ )
    {
        grid[i] =  grid[0] + i*step;
    }
}





double MainWindow::RK4(double x,double y, double h){
    double k1, k2, k3, k4, k;
    double yn = 0;
    double x0 = x;
    double y0 = y;

    k1 = Eq_F(x0, y0);
    k2 = Eq_F((x0 + h/2), (y0 + (h/2) * k1));
    k3 = Eq_F((x0 + h/2), (y0 + (h/2) * k2));
    k4 = Eq_F((x0 + h), (y0 + h * k3));
    k = h*(k1 + 2 * k2 + 2 * k3 + k4)/6;
    yn = y0 + k;

    return yn;
}

double* MainWindow::RK4_Sys(double x, double y1, double y2, double h){
    double K[4][2];
    double* array_of_y = new double[2];

    K[0][0] = Sys_F1(x, y1, y2);
    K[0][1] = Sys_F2(x, y1, y2);
    K[1][0] = h * Sys_F1(x + h/2, y1+K[0][0]/2, y2 + K[0][1]/2);
    K[1][1] = h * Sys_F2(x + h/2, y1+K[0][0]/2, y2 + K[0][1]/2);
    K[2][0] = h * Sys_F1(x + h/2, y1+K[1][0]/2, y2 + K[1][1]/2);
    K[2][1] = h * Sys_F2(x + h/2, y1+K[1][0]/2, y2 + K[1][1]/2);
    K[3][0] = h * Sys_F1(x + h, y1+K[2][0], y2 + K[2][1]);
    K[3][1] = h * Sys_F2(x + h, y1+K[2][0], y2 + K[2][1]);
    array_of_y[0] =y1 + ((K[0][0] + 2 * K[1][0] + 2 * K[2][0] + K[3][0])/6);
    array_of_y[1] =y2 + ((K[0][1] + 2 * K[1][1] + 2 * K[2][1] + K[3][1])/6);

    return array_of_y;
}

double MainWindow::Sys_F1(double x, double y1, double y2){
    return y2;
}

double MainWindow::Sys_F2(double x, double y1, double y2){
    return (-A) * y2 * y2 - B * sin(y1);
}

double MainWindow::Eq_F(double x, double y){
    return ((-5)/2 * y);
}

inline double MainWindow::Eq_F_TrueSol(double x){
    return (exp(((-5)/2) * x));
}



