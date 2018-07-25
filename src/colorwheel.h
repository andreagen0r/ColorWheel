#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include <QWidget>
#include "nkVector3.h"

class ColorWheel : public QWidget
{
    Q_OBJECT

public:
    explicit ColorWheel(QWidget *parent = 0);
    explicit ColorWheel(const QColor in_Color, QWidget *parent = 0);

    QColor getColor() const;

public slots:
    void setColor(const QColor &in_Color);

signals:
    void colorChanged(QColor color);

private:
    bool isHitMode();

    QColor saturationValueAt(const nkn::NknVector3 &in_mouseVec);
    QPointF saturationValueFromColor(const QColor &in_color);
    QColor hueAt(const nkn::NknVector3 &in_mouseVec);

    void drawWheel();
    void drawColorSelected();
    void drawIndicators(QPainter *painter);

    enum class UpDown : int_fast8_t{
        UP = 0,
        DOWN = 1
    };

    UpDown getQuadrant(const QPoint &position);

    enum class HitPosition : int_fast8_t{
        IDLE = 0,
        WHEEL = 1,
        CHOOSER = 2
    };

    UpDown m_quadHit;
    HitPosition m_hitMode;

    QColor m_Color;

    QConicalGradient m_wheelGradient;
    QLinearGradient m_saturationGradient;
    QLinearGradient m_valueGradient;

    nkn::NknVector3 m_mouseVec;

    QPointF m_arrow[3];
    QPointF m_worldCenter;
    QPointF m_indicatorPosition;
    const QPointF m_pointZero{0,0};

    QRectF m_chooserSize;

    double m_innerRadius;
    double m_outerRadius;
    double m_indicatorSize;

    QPixmap m_wheelPixmap;
    QPixmap m_chooserPixmap;

    int m_width;
    int m_height;

    bool isHueChanged;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *) override;
};

#endif // COLORWHEEL_H
