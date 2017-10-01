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

    QColor getColor() const;

public slots:
    void setColor(const QColor &in_Color);

signals:
    void colorChanged(QColor inValue);

private:
    void initialize();
    bool isHitMode();
    auto getQuadrant();
    double angleAt(const Physis::PhVector3 v1, const Physis::PhVector3 v2);

    QColor colorFromPoint(const Physis::PhVector3 &in_mouseVec);

    QPointF pointFromColor(const QColor &in_color);

    QColor colorFromWheel(const Physis::PhVector3 &in_mouseVec);

    void drawWheel();
    void drawColorSelected();
    void drawIndicators(QPainter *painter);

    enum class Quadrant : unsigned char{
        LEFT_UP = 0,
        RIGHT_UP = 1,
        LEFT_DOWN = 2,
        RIGHT_DOWN = 3
    } m_quadHit;

    enum class HitPosition : unsigned char {
        IDLE = 0,
        WHEEL = 1,
        CHOOSER = 2
    } m_hitMode;

    QColor m_Color;

    QConicalGradient m_wheelGradient;
    QLinearGradient m_saturationGradient;
    QLinearGradient m_valueGradient;

    Physis::PhVector3 m_mouseVec;

    QPoint m_arrow[3];
    QPointF m_worldCenter;
    QPointF m_indicatorPosition;

    QRectF m_chooserSize;

    double m_innerRadius;
    double m_outerRadius;
    double m_indicatorSize;

    QPixmap m_wheelPixmap;
    QPixmap m_chooserPixmap;

    int m_width;
    int m_height;

    bool isChooserRefreshed;
    bool isColorRefreshed;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *) override;
};

#endif // COLORWHEEL_H
