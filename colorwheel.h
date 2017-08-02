#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include <QWidget>
#include <QColor>
#include <QGradient>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QVector2D>
#include <QDebug>

#include "pmath.h"

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
    void initialize();
    bool isWheelHit();
    void getQuadrant();
    float getAngle(QVector2D v1, QVector2D v2);

    QColor mColor;

    enum Quadrant {
        LEFT_UP = 0,
        RIGHT_UP = 1,
        LEFT_DOWN = 2,
        RIGHT_DOWN = 3
    };

    QConicalGradient mGradient;
    QPoint arrow[3];
    float mInnerRadius;
    float mOuterRadius;
    QVector2D mMouseVec;
    short mouseQuadTest;
    bool mWheelHit;


protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif // COLORWHEEL_H
