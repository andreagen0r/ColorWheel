/**************************************************************************
**  PHYSIS 3D DCC SOFTWARE
**  ALL RIGHTS RESERVED TO PHYSIS AND YOUR OWNERS
**  Special keywords: andreagenor 8 2017
**  Environment variables:
**  To protect a percent sign, use '%'.
**
**************************************************************************/
#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    connect(ui->widget, SIGNAL(colorChanged(QColor)), this, SLOT(cor(QColor)));
    connect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), ui->widget, SLOT(setHue(double)));

    ui->widget->setColor(Qt::yellow);

    ui->horizontalSlider->setValue(ui->widget->getColor().hslHue());
    ui->horizontalSlider_2->setValue(ui->widget->getColor().saturation());
    ui->horizontalSlider_3->setValue(ui->widget->getColor().value());
}

Form::~Form()
{
    delete ui;
}

void Form::cor(const QColor &inCor)
{
    ui->horizontalSlider->setValue(inCor.hue());
    ui->horizontalSlider_2->setValue(inCor.saturation());
    ui->horizontalSlider_3->setValue(inCor.value());

    ui->doubleSpinBox->setValue(inCor.hslHueF());
    ui->doubleSpinBox_2->setValue(inCor.saturationF());
    ui->doubleSpinBox_3->setValue(inCor.valueF());
}

void Form::on_horizontalSlider_valueChanged(int value)
{
    QColor tempColor = ui->widget->getColor();
    tempColor.setHsv(value, ui->widget->getColor().saturation(), ui->widget->getColor().value());
    ui->widget->setColor(tempColor);
}

void Form::on_horizontalSlider_2_valueChanged(int value)
{
    QColor tempColor = ui->widget->getColor();
    tempColor.setHsv(ui->widget->getColor().hue(), value, ui->widget->getColor().value());
    ui->widget->setColor(tempColor);
}

void Form::on_horizontalSlider_3_valueChanged(int value)
{
    QColor tempColor = ui->widget->getColor();
    tempColor.setHsv(ui->widget->getColor().hue(), ui->widget->getColor().saturation(), value);
    ui->widget->setColor(tempColor);
}

void Form::on_doubleSpinBox_valueChanged(double arg1)
{
    QColor tempColor = ui->widget->getColor();
    tempColor.setHsvF(arg1, ui->widget->getColor().saturationF(), ui->widget->getColor().valueF());
    ui->widget->setColor(tempColor);
}

void Form::on_doubleSpinBox_2_valueChanged(double arg1)
{
    QColor tempColor = ui->widget->getColor();
    tempColor.setHsvF(ui->widget->getColor().hueF(), arg1, ui->widget->getColor().valueF());
    ui->widget->setColor(tempColor);
}

void Form::on_doubleSpinBox_3_valueChanged(double arg1)
{
    QColor tempColor = ui->widget->getColor();
    tempColor.setHsvF(ui->widget->getColor().hueF(), ui->widget->getColor().saturationF(), arg1);
    ui->widget->setColor(tempColor);
}
