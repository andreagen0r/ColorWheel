#include "colorwheel.h"
#include "nkMath.h"

#include <QColor>
#include <QGradient>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include <cmath>

ColorWheel::ColorWheel(QWidget *parent)
    : ColorWheel(Qt::red, parent)
{
}

ColorWheel::ColorWheel(const QColor &in_Color, QWidget *parent)
    : QWidget(parent),
      m_quadHit(UpDown::UP),
      m_hitMode(HitPosition::IDLE),
      m_Color(in_Color),
      m_innerRadius(0),
      m_outerRadius(0),
      m_indicatorSize(0),
      m_width(0),
      m_height(0),
      isHueChanged(false)
{
    setFocusPolicy(Qt::StrongFocus);

    setMouseTracking(true);

    // Wheel gradient
    m_wheelGradient.setAngle(0.0);
    m_wheelGradient.setCenter(m_pointZero);
    m_wheelGradient.setColorAt(0.0, Qt::red);
    m_wheelGradient.setColorAt(60.0 /360.0, Qt::yellow);
    m_wheelGradient.setColorAt(120.0 /360.0, Qt::green);
    m_wheelGradient.setColorAt(180.0 /360.0, Qt::cyan);
    m_wheelGradient.setColorAt(240.0 /360.0, Qt::blue);
    m_wheelGradient.setColorAt(300.0 /360.0, Qt::magenta);
    m_wheelGradient.setColorAt(1.0, Qt::red);

    // Sampler gradient
    m_valueGradient.setColorAt(0.0, Qt::transparent);
    m_valueGradient.setColorAt(1.0, Qt::black);
}

QColor ColorWheel::getColor() const
{
    return m_Color;
}

void ColorWheel::setColor(const QColor &in_Color)
{
    if(in_Color == m_Color)
    {
        isHueChanged = false;
        return;
    }

    isHueChanged = true;

    m_Color = in_Color;
    emit colorChanged(m_Color);

    if(isHueChanged)
    {
        drawColorSelected();
        isHueChanged = false;
    }

    update();
}

bool ColorWheel::isHitMode()
{
    if(static_cast<double>(m_mouseVec.length()) > m_innerRadius && static_cast<double>(m_mouseVec.length()) < m_outerRadius)
    {
        m_hitMode = HitPosition::WHEEL;
        return true;
    }

    if(m_chooserSize.contains(static_cast<double>(m_mouseVec.x()), static_cast<double>(m_mouseVec.y())))
    {
        m_hitMode = HitPosition::CHOOSER;
        return true;
    }

    return false;
}

ColorWheel::UpDown ColorWheel::getQuadrant(const QPoint &position)
{
    UpDown output;

    if(position.y() <= static_cast<int>(m_height/2))
    {
        output = UpDown::UP;
    }
    else
    {
        output = UpDown::DOWN;
    }

    return output;
}

QColor ColorWheel::saturationValueAt(const QVector2D &in_mouseVec)
{
    double x{static_cast<double>(in_mouseVec.x())};
    double y{static_cast<double>(-in_mouseVec.y())};

    if(!m_chooserSize.contains(static_cast<double>(in_mouseVec.x()), static_cast<double>(in_mouseVec.y())))
    {
        // Test Saturation X axis
        if(static_cast<double>(in_mouseVec.x()) <= m_chooserSize.left())
        {
            x = m_chooserSize.left();
        }
        else if (static_cast<double>(in_mouseVec.x()) >= m_chooserSize.right())
        {
            x = m_chooserSize.right();
        }

        // Test Value Y axis
        if(static_cast<double>(in_mouseVec.y()) <= m_chooserSize.top())
        {
            y = -m_chooserSize.top();
        }
        else if (static_cast<double>(in_mouseVec.y()) >= m_chooserSize.bottom())
        {
            y = -m_chooserSize.bottom();
        }
    }

    double m_sat = (x / m_chooserSize.width()) + 0.5;
    double m_val = (y / m_chooserSize.height()) + 0.5;

    QColor color;
    color.setHsvF(m_Color.hsvHueF(), m_sat, m_val);
    return color;
}

QPointF ColorWheel::saturationValueFromColor(const QColor &in_color)
{
    return QPointF{m_chooserSize.width() * (in_color.saturationF() - 0.5),
                   -m_chooserSize.height() * (in_color.valueF() - 0.5)};
}

QColor ColorWheel::hueAt(const QVector2D &in_mouseVec)
{
    QVector2D vec{1.0, 0.0};
    float angle = nkn::math::radiansToDegrees(
                std::acos(in_mouseVec.dotProduct(vec, in_mouseVec) / (in_mouseVec.length() * vec.length()))
                );

    m_quadHit = getQuadrant(mapFromGlobal(QCursor::pos()));

    if(m_quadHit == UpDown::DOWN)
    {
        angle = 360 - angle;
    }

    QColor color;
    color.setHsvF(static_cast<double>(angle) / 360, m_Color.hsvSaturationF(), m_Color.valueF());

    return color;
}

void ColorWheel::drawWheel()
{
    m_wheelPixmap = QPixmap(m_width, m_height);
    m_wheelPixmap.fill(QColor(0,0,0,0));

    QPainterPath path;
    QPainter painter(&m_wheelPixmap);
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.translate(m_worldCenter.x(), m_worldCenter.y());
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_wheelGradient);
    path.addEllipse(m_pointZero, m_innerRadius, m_innerRadius);
    path.addEllipse(m_pointZero, m_outerRadius, m_outerRadius);
    painter.drawPath(path);

    // Draw chooser foreground
    painter.setRenderHints(QPainter::Antialiasing, false);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    painter.setBrush(m_valueGradient);
    painter.drawRect(m_chooserSize);
}

void ColorWheel::drawColorSelected()
{
    m_chooserPixmap = QPixmap(static_cast<int>(m_chooserSize.width()), static_cast<int>(m_chooserSize.height()));
    m_chooserPixmap.fill(QColor(0,0,0,0));

    QColor pureColor;
    pureColor.setHsvF(m_Color.hsvHueF(), 1.0, 1.0, 1.0);

    m_saturationGradient.setColorAt(1.0, pureColor);
    m_saturationGradient.setColorAt(0, Qt::white);

    QPainter p(&m_chooserPixmap);
    p.setRenderHints(QPainter::Antialiasing, false);
    p.setPen(QPen(Qt::black, 1, Qt::NoPen));
    p.setBrush(m_saturationGradient);
    p.drawRect(QRectF(m_pointZero, QSizeF(m_chooserSize.width(), m_chooserSize.height())));
}

void ColorWheel::drawIndicators(QPainter *painter)
{
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->translate(m_worldCenter.x(), m_worldCenter.y());

    // Rotate Indicator
    painter->save();
    painter->rotate(-m_Color.hsvHue());

    // Draw wheel indicator
    painter->setBrush(Qt::black);
    painter->drawConvexPolygon(m_arrow, 3);
    painter->drawLine(QPointF(m_innerRadius + 1.0, 0), QPointF(m_outerRadius, 0));
    painter->restore();

    // Draw chooser indicator
    QPointF indicatorPosition = saturationValueFromColor(m_Color);

    painter->drawEllipse(indicatorPosition, m_indicatorSize, m_indicatorSize);
    painter->setBrush(m_Color);
    painter->setPen(QPen(Qt::white, 1, Qt::SolidLine));
    painter->drawEllipse(indicatorPosition, m_indicatorSize - 1.0, m_indicatorSize - 1.0);
}

void ColorWheel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    double pMapX{static_cast<double>(m_width) / 2 - (m_chooserSize.width() / 2)};
    double pMapY{static_cast<double>(m_height) / 2 - (m_chooserSize.height() / 2)};

    painter.drawPixmap(static_cast<int>(pMapX), static_cast<int>(pMapY),m_chooserPixmap);

    painter.drawPixmap(m_pointZero, m_wheelPixmap);
    drawIndicators(&painter);
}

void ColorWheel::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() != Qt::LeftButton)
    {
        return;
    }

    m_mouseVec.setX(event->x() - static_cast<float>(m_width) / 2);
    m_mouseVec.setY(event->y() - static_cast<float>(m_height) / 2);

    if(!isHitMode())
    {
        return;
    }

    switch (m_hitMode)
    {
        case HitPosition::WHEEL:
        {
            setColor(hueAt(m_mouseVec));
            break;
        }
        case HitPosition::CHOOSER:
        {
            setColor(saturationValueAt(m_mouseVec));
            setCursor(Qt::BlankCursor);
            break;
        }
        default:
        {} break;
    }
}

void ColorWheel::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_hitMode = HitPosition::IDLE;
    setCursor(Qt::ArrowCursor);
}

void ColorWheel::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() != Qt::LeftButton)
    {
        return;
    }

    if(m_hitMode == HitPosition::IDLE)
    {
        return;
    }

    m_mouseVec.setX(event->x() - static_cast<float>(m_width) / 2);
    m_mouseVec.setY(event->y() - static_cast<float>(m_height) / 2);

    switch (m_hitMode)
    {
        case HitPosition::WHEEL:
        {
            setColor(hueAt(m_mouseVec));
            break;
        }
        case HitPosition::CHOOSER:
        {
            setColor(saturationValueAt(m_mouseVec));
            break;
        }
        default:
        {} break;
    }
}

void ColorWheel::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    m_width = width();
    m_height = height();

    // Move world origin to center of screen
    m_worldCenter.setX(static_cast<int>(m_width / 2));
    m_worldCenter.setY(static_cast<int>(m_height / 2));

    double side = std::fmin(width(), height()) / 2;
    m_outerRadius = side - 1.0;
    m_innerRadius = side - (side * 0.25);

    // Make the arrow points relative to widget size
   m_arrow [0] = QPointF(m_innerRadius, 3 * (side * 0.01));
   m_arrow [1] = QPointF(m_innerRadius + 6 * (side * 0.01), 0);
   m_arrow [2] = QPointF(m_innerRadius, -3 * (side * 0.01));

   // Calculate the size for sampler color
   double diagonal = std::cos(nkn::math::degreeToRadians(45.0)) * m_innerRadius;
   double gap = (diagonal * 0.05);

   // Calculate chooser indicator size
   m_indicatorSize = m_innerRadius * 0.04;
   if(m_indicatorSize < 1.0)
   {
       m_indicatorSize = 1.0;
   }

   // Setup the rect size for color sampler
   m_chooserSize.setTopLeft(QPointF( -diagonal + gap, -diagonal + gap));
   m_chooserSize.setBottomRight(QPointF( diagonal - gap, diagonal - gap));

   m_saturationGradient.setStart(m_pointZero);
   m_saturationGradient.setFinalStop(QPointF(m_chooserSize.width(), 0));

   m_valueGradient.setStart(QPointF(0, -diagonal + gap));
   m_valueGradient.setFinalStop(QPointF(0, diagonal - gap));

   drawWheel();
   saturationValueFromColor(m_Color);
   drawColorSelected();
}
