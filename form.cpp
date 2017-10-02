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
    connect(ui->widget, &ColorWheel::colorChanged, this, &Form::cor);

//    ui->widget->setColor(QColor(190, 108, 228));
    ui->widget->setColor(Qt::darkCyan);

    QColor color = ui->widget->getColor();

    ui->horizontalSlider->setValue(color.hue());
    ui->horizontalSlider_2->setValue(color.saturation());
    ui->horizontalSlider_3->setValue(color.value());
}

Form::~Form()
{
    delete ui;
}

void Form::cor(const QColor &inCor)
{
    m_color = inCor;
    ui->horizontalSlider->setValue(inCor.hue());
    ui->horizontalSlider_2->setValue(inCor.saturation());
    ui->horizontalSlider_3->setValue(inCor.value());

//    ui->doubleSpinBox->setValue(inCor.hslHueF());
//    ui->doubleSpinBox_2->setValue(inCor.saturationF());
//    ui->doubleSpinBox_3->setValue(inCor.valueF());
}

void Form::on_horizontalSlider_valueChanged(int value)
{
    QColor tempColor = m_color;
    tempColor.setHsv(value, m_color.hsvSaturation(), m_color.value());
    ui->widget->setColor(tempColor);
}

void Form::on_horizontalSlider_2_valueChanged(int value)
{
    QColor tempColor = m_color;
    tempColor.setHsv(m_color.hsvHue(), value, m_color.value());
    ui->widget->setColor(tempColor);
}

void Form::on_horizontalSlider_3_valueChanged(int value)
{
    QColor tempColor = m_color;
    tempColor.setHsv(m_color.hsvHue(), m_color.hsvSaturation(), value);
    ui->widget->setColor(tempColor);
}
