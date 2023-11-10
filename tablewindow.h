#ifndef TABLEWINDOW_H
#define TABLEWINDOW_H

#include <QDialog>
#include "tmessage.h"

namespace Ui {
class tablewindow;
}

class tablewindow : public QDialog
{
    Q_OBJECT

public:
    explicit tablewindow(QWidget *parent = nullptr);
    ~tablewindow();
public slots:
    void slot(TMessage& message);


private:
    Ui::tablewindow *ui;
};

#endif // TABLEWINDOW_H
