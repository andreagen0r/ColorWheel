#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include <QWidget>
#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QGradient>
#include <QMouseEvent>
#include <QtMath>
#include <QVector2D>

#include <QDebug>

#include <cmath>

class ColorWheel : public QWidget
{
    Q_OBJECT

public:
    explicit ColorWheel(QWidget *parent = 0);
    explicit ColorWheel(const QColor &inColor, QWidget *parent = 0);
    ~ColorWheel();

public slots:
    QColor getColor() const;
    void setColor(const QColor &inValue);

signals:
    void colorChanged(QColor inValue);

private:
    QColor mColor;

    enum {
        LEFT_UP = 0,
        RIGHT_UP = 1,
        LEFT_DOWN = 2,
        RIGHT_DOWN = 3
    };

    QConicalGradient mGradient;

    QVector2D mMouseVec;
    QPoint arrow[3];

    double mInnerRadius;
    double mOuterRadius;
    double mHue;
    int mouseQuadTest;
    bool mWheelHit;

    bool isHit();
    double getAngle(QVector2D v1, QVector2D v2);
    void mouseCalc();

    double getHue() const;
    void setHue(double value);
    void changeColor();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif // COLORWHEEL_H
