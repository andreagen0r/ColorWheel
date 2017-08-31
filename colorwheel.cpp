#include "colorwheel.h"

ColorWheel::ColorWheel(QWidget *parent) :
    QWidget(parent)
{
    setColor(Qt::red);
    initialize();
}

ColorWheel::ColorWheel(QColor inColor, QWidget *parent) :
    QWidget(parent)
{

    setColor(inColor);
    initialize();
}

ColorWheel::~ColorWheel()
{

}

QColor ColorWheel::getColor() const
{
    return m_Color;
}

void ColorWheel::setColor(const QColor &inColor)
{
    m_Color = inColor;
    emit colorChanged(m_Color);
    update();
}

void ColorWheel::setColor(float inHue, float inSaturation, float inValue, float inAlpha)
{
    QColor inColor(Qt::black);
    inColor.setHsvF(inHue, inSaturation, inValue, inAlpha);
    m_Color = inColor;
    emit colorChanged(m_Color);
    update();
}

void ColorWheel::setHue(float inHue)
{
    QColor inColor(Qt::black);
    inColor.setHsvF(inHue, m_Color.saturationF(), m_Color.valueF(), m_Color.alphaF());
    m_Color = inColor;
    emit colorChanged(m_Color);
    update();
}

void ColorWheel::setSaturation(float inSaturation)
{
    QColor inColor(Qt::black);

    inColor.setHsvF(m_Color.hueF(), inSaturation, m_Color.valueF(), m_Color.alphaF());
    m_Color = inColor;
    emit colorChanged(m_Color);
    update();
}
void ColorWheel::setValue(float inValue)
{
    QColor inColor(Qt::black);
    inColor.setHsvF(m_Color.hueF(), m_Color.saturationF(), inValue, m_Color.alphaF());
    m_Color = inColor;
    emit colorChanged(m_Color);
    update();
}

void ColorWheel::setAlpha(float inAlpha)
{
    QColor inColor(Qt::black);
    inColor.setHsvF(m_Color.hueF(), m_Color.saturationF(), m_Color.valueF(), inAlpha);
    m_Color = inColor;
    emit colorChanged(m_Color);
    update();
}

void ColorWheel::initialize()
{
    // Wheel gradient
    m_Gradient.setAngle(0.0);
    m_Gradient.setCenter(QPointF(0.0, 0.0));
    m_Gradient.setColorAt(0.0, Qt::red);
    m_Gradient.setColorAt(60.0 /360.0, Qt::yellow);
    m_Gradient.setColorAt(120.0 /360.0, Qt::green);
    m_Gradient.setColorAt(180.0 /360.0, Qt::cyan);
    m_Gradient.setColorAt(240.0 /360.0, Qt::blue);
    m_Gradient.setColorAt(300.0 /360.0, Qt::magenta);
    m_Gradient.setColorAt(1.0, Qt::red);

    // Sampler gradient
    m_ValueGradient.setColorAt(0.0, QColor(0,0,0,0));
    m_ValueGradient.setColorAt(1.0, Qt::black);
}

bool ColorWheel::isWheelHit()
{
    return (m_MouseVec.length() > m_InnerRadius && m_MouseVec.length() < m_OuterRadius);
}

bool ColorWheel::isQuadHit()
{
    return (m_ChooserSize.left() <= m_MouseVec.x && m_ChooserSize.right() >= m_MouseVec.x
            && m_ChooserSize.top() <= m_MouseVec.y && m_ChooserSize.bottom() >= m_MouseVec.y);
}

unsigned char ColorWheel::getQuadrant()
{
    QPoint mousePosition = mapFromGlobal(QCursor::pos());

    if(mousePosition.x() < width()/2)
    {

        if(mousePosition.y() < height()/2)
        {
            return LEFT_UP;
        }
        else
        {
            return LEFT_DOWN;
        }
    }
    else
    {
        if(mousePosition.y() < height()/2)
        {
            return RIGHT_UP;
        }
        else
        {
            return RIGHT_DOWN;
        }
    }
}

float ColorWheel::calcAngle(Physis::PhVector3 v1, Physis::PhVector3 v2)
{
    float angle = physis::math::radiansToDegrees(std::acos( v1.dot(v2) / (v1.length() * v2.length() ) ));

    if(getQuadrant() == LEFT_DOWN || getQuadrant() == RIGHT_DOWN)
    {
        angle = 360 - angle;
    }

    return angle;
}

void ColorWheel::wheelUpdate()
{
    if(m_WheelHit)
    {
        setHue((calcAngle(m_MouseVec.normalize(), Physis::PhVector3(1,0,0).normalize())) / 360);
    }
}

 void ColorWheel::chooserUpdate()
 {
     if(m_QuadHit)
     {
         int x;
         int y;

         // Test Saturation X axis
         if(m_MouseVec.x < m_ChooserSize.left())
         {
             x = m_ChooserSize.left();
         }
         else if (m_MouseVec.x > m_ChooserSize.right())
         {
             x = m_ChooserSize.right();
         }
         else
         {
             x = m_MouseVec.x;
         }

         // Test Value Y axis
         if(m_MouseVec.y < m_ChooserSize.top())
         {
             y = -m_ChooserSize.top();
         }
         else if (m_MouseVec.y > m_ChooserSize.bottom())
         {
             y = -m_ChooserSize.bottom();
         }
         else
         {
             y = -m_MouseVec.y;
         }



         // Setup saturation
         if (x == 0)
         {
             setSaturation(0.5);
         }
         else
         {
             setSaturation((x / m_ChooserSize.width()) + 0.5);
         }


         // Setup value
         if (y == 0)
         {
             setValue(0.5);
         }
         else
         {
             setValue( (y / m_ChooserSize.height()) + 0.5);
         }
     }

     indicatorUpdate();
 }

void ColorWheel::indicatorUpdate()
{
    if(getColor().saturationF() == 0.5)
    {
        m_IndicatorPosition.setX(0);
    }
    else
    {
        m_IndicatorPosition.setX(m_Color.saturationF() * m_ChooserSize.width() - (m_ChooserSize.width() / 2));
    }

    if(getColor().valueF() == 0.5)
    {
        m_IndicatorPosition.setY(0);
    }
    else
    {
        m_IndicatorPosition.setY(-(m_Color.valueF() * m_ChooserSize.height() - (m_ChooserSize.height() / 2)));
    }
}

void ColorWheel::drawWheel(QPainter *inPainter)
{
    QPainterPath path;
    inPainter->setRenderHints(QPainter::Antialiasing, true);
    inPainter->setPen(Qt::NoPen);
    inPainter->setBrush(m_Gradient);
    path.addEllipse(QPointF(0.0f, 0.0f), m_InnerRadius, m_InnerRadius);
    path.addEllipse(QPointF(0.0f, 0.0f), m_OuterRadius, m_OuterRadius);
    inPainter->drawPath(path);
}

void ColorWheel::drawChooser(QPainter *inPainter)
{
    inPainter->setRenderHints(QPainter::Antialiasing, false);
    QColor pureColor;
    pureColor.setHsvF(getColor().hueF(), 0.0f, 1.0f, 1.0f);

    QColor valueColor(Qt::black);
    valueColor.setHsvF(getColor().hueF(), 1.0f, 1.0f, 1.0f);
    m_SaturationGradient.setColorAt(1.0f, valueColor);
    m_SaturationGradient.setColorAt(0.0f, pureColor);

    inPainter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    inPainter->setBrush(m_SaturationGradient);
    inPainter->drawRect(m_ChooserSize);
    inPainter->setBrush(m_ValueGradient);
    inPainter->drawRect(m_ChooserSize );
}

void ColorWheel::drawIndicators(QPainter *inPainter)
{
    inPainter->setRenderHints(QPainter::Antialiasing, true);
    inPainter->setPen(QPen(Qt::black,1, Qt::SolidLine));

    // Rotate Indicator
    inPainter->save();
    inPainter->rotate(-getColor().hueF() * 360);

    // Draw wheel indicator arrow
    inPainter->drawLine(QPointF(m_InnerRadius + 1.0f, 0.0f), QPointF(m_OuterRadius, 0.0f));
    inPainter->setPen(Qt::NoPen);
    inPainter->setBrush(Qt::black);
    inPainter->translate(m_InnerRadius, 0.0f);
    inPainter->drawConvexPolygon(m_Arrow, 3);
    inPainter->setClipping(false);
    inPainter->restore();

    // Draw chooser indicator
    inPainter->setBrush(m_Color);
    inPainter->setPen(QPen(Qt::black, m_IndicatorBorder, Qt::SolidLine));
    inPainter->drawEllipse(m_IndicatorPosition, m_IndicatorSize, m_IndicatorSize);
    inPainter->setBrush(Qt::NoBrush);
    inPainter->setPen(QPen(Qt::white, 1, Qt::SolidLine));
    inPainter->drawEllipse(m_IndicatorPosition, m_IndicatorSize -1 , m_IndicatorSize - 1);
}

void ColorWheel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    // Move the world to the center of the screen
    painter.translate(m_WorldCenter);
    drawWheel(&painter);
    drawChooser(&painter);

    indicatorUpdate();
    drawIndicators(&painter);

    // Debug color output;
//    painter.end();
//    painter.begin(this);
//    painter.setBrush(getColor());
//    painter.drawRect(0, 0, 60, 60);
}

void ColorWheel::mousePressEvent(QMouseEvent *event)
{
    m_MouseVec.x = event->x() - width() / 2.0f;
    m_MouseVec.y = event->y() - height() / 2.0f;

    m_WheelHit = isWheelHit();
    m_QuadHit = isQuadHit();

    if(m_WheelHit)
    {
      wheelUpdate();
      return;
    }

    if(m_QuadHit)
    {
        setCursor(Qt::UpArrowCursor);
        chooserUpdate();
        return;
    }
}

void ColorWheel::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    setCursor(Qt::ArrowCursor);
}

void ColorWheel::mouseMoveEvent(QMouseEvent *event)
{
    m_MouseVec.x = event->x() - width() / 2.0f;
    m_MouseVec.y = event->y() - height() / 2.0f;

    if(m_WheelHit)
    {
        wheelUpdate();
        return;
    }

    if(m_QuadHit)
    {
        chooserUpdate();
        return;
    }
}

void ColorWheel::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    // Move world origin to center of screen
    m_WorldCenter.setX(width() / 2.0f);
    m_WorldCenter.setY(height() / 2.0f);

    int side = std::min(width(), height()) / 2;
    m_OuterRadius = side - 1.0f;
    m_InnerRadius = side - (side * 0.25f);

    // Make the arrow points relative to widget size
   m_Arrow [0] = QPoint(0, 3 * (side * 0.01));
   m_Arrow [1] = QPoint(6 * (side * 0.01), 0);
   m_Arrow [2] = QPoint(0, -3 * (side * 0.01));

   // Calculate the size for sampler color
   float diagonal = std::cos(physis::math::degreeToRadians(45.0f)) * m_InnerRadius;
   float gap = (diagonal * 0.05f);

   // Calculate chooser indicator border size
   (std::min(width(), height()) < 500) ? m_IndicatorBorder = 1 : m_IndicatorBorder = 2;

   // Calculate chooser indicator size
   m_IndicatorSize = m_InnerRadius * 0.04f;
   (m_IndicatorSize < 1) ? m_IndicatorSize = 1 : m_IndicatorSize;

   // Setup the rect size for color sampler
   m_ChooserSize.setTopLeft(QPoint( -diagonal + gap, -diagonal + gap ));
   m_ChooserSize.setBottomRight(QPoint( diagonal - gap, diagonal - gap));

   m_SaturationGradient.setStart(QPointF(-diagonal + gap, 0 ));
   m_SaturationGradient.setFinalStop(QPointF(diagonal - gap, 0));

   m_ValueGradient.setStart(QPointF(0, -diagonal + gap ));
   m_ValueGradient.setFinalStop(QPointF(0, diagonal - gap));

   indicatorUpdate();
}
