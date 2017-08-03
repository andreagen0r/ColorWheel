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
    void setColor(const QColor &inColor);
    void setColor(const float &inHue, const float &inSaturation, const float &inValue, const float &inAlpha);
    void setHue(const float &inHue);
    void setSaturation(const float &inSaturation);
    void setValue(const float &inValue);
    void setAlpha(const float &inAlpha);

signals:
    void colorChanged(QColor inValue);

private:
    void initialize();
    bool isWheelHit();
    bool isQuadHit();
    short getQuadrant();
    float calcAngle(QVector2D v1, QVector2D v2);

    void chooserUpdate();
    void indicatorUpdate();
    void hue();

    void drawWheel(QPainter *inPainter);
    void drawChooser(QPainter *inPainter);
    void drawIndicators(QPainter *inPainter);

    QColor mColor;

    enum Quadrant {
        LEFT_UP = 0,
        RIGHT_UP = 1,
        LEFT_DOWN = 2,
        RIGHT_DOWN = 3
    };

    QConicalGradient mGradient;
    QLinearGradient saturationGradient;
    QLinearGradient valueGradient;
    QPoint arrow[3];
    float mInnerRadius;
    float mOuterRadius;
    QVector2D mMouseVec;
    QPointF mWorldCenter;

    bool mWheelHit;
    bool mQuadHit;
    QRectF mChooserSize;

    QPoint mIndicatorPosition;
    int indicatorSize;
    int indicatorBorder;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif // COLORWHEEL_H
