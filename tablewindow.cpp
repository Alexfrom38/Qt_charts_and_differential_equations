#include "tablewindow.h"
#include "ui_tablewindow.h"
#include <iostream>
tablewindow::tablewindow(QWidget *parent) : QDialog(parent), ui(new Ui::tablewindow)
{
    ui->setupUi(this);

}

tablewindow::~tablewindow()
{
    delete ui;
}

void tablewindow::slot(TMessage& message)
{

    QString string = "H";
    message.print();
    ui->label->setText(string);
}


