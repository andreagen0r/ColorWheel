#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include <QWidget>
#include <QColor>
#include <QGradient>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
//#include <QVector2D>

#include <QDebug>

#include "phmath.h"
#include "phvector3.h"

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
    unsigned char getQuadrant();
    float calcAngle(Physis::PhVector3 v1, Physis::PhVector3 v2);

    void wheelUpdate();
    void chooserUpdate();
    void indicatorUpdate();

    void drawWheel(QPainter *inPainter);
    void drawChooser(QPainter *inPainter);
    void drawIndicators(QPainter *inPainter);

    enum Quadrant : unsigned char {
        LEFT_UP = 0,
        RIGHT_UP = 1,
        LEFT_DOWN = 2,
        RIGHT_DOWN = 3
    };

    QColor m_Color;
    QConicalGradient m_Gradient;
    QLinearGradient m_SaturationGradient;
    QLinearGradient m_ValueGradient;
    QPoint m_Arrow[3];
    float m_InnerRadius;
    float m_OuterRadius;
    Physis::PhVector3 m_MouseVec;
    QPointF m_WorldCenter;

    bool m_WheelHit;
    bool m_QuadHit;
    QRectF m_ChooserSize;

    QPoint m_IndicatorPosition;
    short m_IndicatorSize;
    short m_IndicatorBorder;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif // COLORWHEEL_H
