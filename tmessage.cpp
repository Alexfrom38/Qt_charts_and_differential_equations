#include "tmessage.h"
#include <QVector>
#include<QDebug>
TMessage::TMessage()
{

    count_of_iter = 0;
}

void TMessage::print()
{
    double a = 0;
    QVectorIterator<double> itNumbers(array_of_xi);
    while (itNumbers.hasNext())
        std::cout<< (itNumbers.next())<<"FDFG"<<std::endl;
}

size_t TMessage::GetCount()
{
    return count_of_iter;
}

void TMessage::clear()
{
    array_of_xi.clear();
    array_of_vi.clear();
    array_of_v2i.clear();
    OLP.clear();
    hi.clear();
    C1.clear();
    C2.clear();
    Ui.clear();
    Ui_Vi.clear();
}
void TMessage::Add_Elem(bool f_IsTest, double f_Xi,double f_Vi,double f_V2i,double f_Sub1, double f_OLP, double f_hi, double f_C1, double f_C2, double f_Ui, double f_Sub2)
{
    IsTest = f_IsTest;
    array_of_xi.push_back(f_Xi);
    array_of_vi.push_back(f_Vi);
    array_of_v2i.push_back(f_V2i);
    sub_vi_v2i.push_back(f_Sub1);
    OLP.push_back(f_OLP);
    hi.push_back(f_hi);
    C1.push_back(f_C1);
    C2.push_back(f_C2);
    count_of_iter++;

}


