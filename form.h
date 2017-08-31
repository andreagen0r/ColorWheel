/**************************************************************************
**  PHYSIS 3D DCC SOFTWARE
**  ALL RIGHTS RESERVED TO PHYSIS AND YOUR OWNERS
**  Special keywords: andreagenor 8 2017
**  Environment variables:
**  To protect a percent sign, use '%'.
**
**************************************************************************/
#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QColor>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private slots:
    void cor(const QColor &inCor);

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_horizontalSlider_3_valueChanged(int value);

//    void on_doubleSpinBox_valueChanged(double arg1);

//    void on_doubleSpinBox_2_valueChanged(double arg1);

//    void on_doubleSpinBox_3_valueChanged(double arg1);

private:
    Ui::Form *ui;
};

#endif // FORM_H
