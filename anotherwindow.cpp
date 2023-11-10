#include "anotherwindow.h"
#include "ui_anotherwindow.h"

AnotherWindow::AnotherWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnotherWindow)
{
    auto placeholder = ui->setupUi;
    placeholder(this);
}

AnotherWindow::~AnotherWindow()
{
    delete ui;
}

void AnotherWindow::on_pushButton_clicked()
{
    this->close();      // Закрываем окно
    emit firstWindow(); // И вызываем сигнал на открытие главного окна
}
