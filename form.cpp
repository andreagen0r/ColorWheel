#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    connect(ui->widget, &ColorWheel::colorChanged, this, &Form::cor);
    ui->widget->setColor(QColor(190, 108, 228));

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
    connect(ui->widget, &ColorWheel::colorChanged, this, &Form::cor);
    ui->horizontalSlider->setValue(inCor.hue());
    ui->horizontalSlider_2->setValue(inCor.saturation());
    ui->horizontalSlider_3->setValue(inCor.value());
    disconnect(ui->widget, &ColorWheel::colorChanged, this, &Form::cor);
}

void Form::on_horizontalSlider_valueChanged(int value)
{
    QColor tempColor;
    tempColor.setHsv(value, ui->widget->getColor().saturation(), ui->widget->getColor().value());
    ui->widget->setColor(tempColor);
}

void Form::on_horizontalSlider_2_valueChanged(int value)
{
    QColor tempColor;
    tempColor.setHsv(ui->widget->getColor().hsvHue(), value, ui->widget->getColor().value());
    ui->widget->setColor(tempColor);
}

void Form::on_horizontalSlider_3_valueChanged(int value)
{
    QColor tempColor;
    tempColor.setHsv(ui->widget->getColor().hsvHue(),ui->widget->getColor().hsvSaturation(), value);
    ui->widget->setColor(tempColor);
}
