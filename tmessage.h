#ifndef TMESSAGE_H
#define TMESSAGE_H
#include<iostream>
#include <QVector>
class TMessage
{
protected:

   size_t count_of_iter;
   bool IsTest = true;
   QVector<double> array_of_xi ;
   QVector<double> array_of_vi ;
   QVector<double> array_of_v2i;
   QVector<double> sub_vi_v2i ;
   QVector<double> OLP ;
   QVector<double> hi;
   QVector<double> C1 ;
   QVector<double> C2 ;
   QVector<double> Ui;
   QVector<double> Ui_Vi;
public:
    TMessage();
    void Add_Elem(bool f_IsTest, double f_Xi,double f_Vi,double f_V2i,double f_Sub1, double f_OLP, double f_hi, double f_C1, double f_C2, double f_Ui = 0, double f_Sub2 = 0);
    void print();
    size_t GetCount();
    void clear();



};

#endif // TMESSAGE_H
