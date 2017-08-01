#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include <QWidget>
#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QGradient>
#include <QMouseEvent>
//#include <QtMath>
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

    void initialize();

    enum {
        LEFT_UP = 0,
        RIGHT_UP = 1,
        LEFT_DOWN = 2,
        RIGHT_DOWN = 3
    };

    QConicalGradient mGradient;

    QVector2D mMouseVec;
    QPoint arrow[3];

    float mInnerRadius;
    float mOuterRadius;
    float mHue;
    short mouseQuadTest;
    bool mWheelHit;

    bool isHit();
    float getAngle(QVector2D v1, QVector2D v2);
    void mouseCalc();

    float getHue() const;
    void setHue(const float &inValue);
    void changeColor();



protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif // COLORWHEEL_H
