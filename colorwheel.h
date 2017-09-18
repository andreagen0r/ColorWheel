#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include <QWidget>
#include <QColor>
#include <QGradient>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QPixmap>
#include <QDebug>

#include "phmath.h"
#include "phvector3.h"

class ColorWheel : public QWidget
{
    Q_OBJECT

public:
    explicit ColorWheel(QWidget *parent = 0);
    explicit ColorWheel(const QColor in_Color, QWidget *parent = 0);
    ~ColorWheel();

public slots:
    QColor getColor() const;
    void setColor(const QColor &in_Color);
    void setColor(const double in_Hue, const double in_Saturation, const double in_Value, const double in_Alpha = 1.0);
    void setHue(const double in_Hue);
    void setSaturation(const double in_Saturation);
    void setValue(const double in_Value);
    void setAlpha(const double in_Alpha);

signals:
    void colorChanged(QColor inValue);

private:
    void initialize();
    bool isWheelHit();
    bool isQuadHit();
    unsigned char getQuadrant();
    double angleAt(Physis::PhVector3 v1, Physis::PhVector3 v2);
    void wheelUpdate();
    void chooserUpdate();
    void indicatorUpdate();

    void drawWheel();
    void drawColor();
    void drawIndicators();

    enum Quadrant : unsigned char {
        LEFT_UP = 0,
        RIGHT_UP = 1,
        LEFT_DOWN = 2,
        RIGHT_DOWN = 3
    };

    QColor m_Color;

    QConicalGradient m_WheelGradient;
    QLinearGradient m_SaturationGradient;
    QLinearGradient m_ValueGradient;

    Physis::PhVector3 m_MouseVec;

    QPointF m_Arrow[3];
    QPointF m_WorldCenter;
    QPointF m_IndicatorPosition;

    bool m_WheelHit;
    bool m_ChooserHit;

    QRectF m_ChooserSize;

    double m_InnerRadius;
    double m_OuterRadius;
    double m_IndicatorSize;

    QPixmap m_WheelPixmap;
    QPixmap m_ChooserPixmap;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *) override;
};

#endif // COLORWHEEL_H
