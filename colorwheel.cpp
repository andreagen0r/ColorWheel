#include "colorwheel.h"

static int countWheel;
static int countChooser;

ColorWheel::ColorWheel(QWidget *parent) :
    QWidget(parent), m_Color(Qt::red)
{
    initialize();
}

ColorWheel::ColorWheel(const QColor in_Color, QWidget *parent) :
    QWidget(parent), m_Color(in_Color)
{
    initialize();
}

ColorWheel::~ColorWheel()
{

}

QColor ColorWheel::getColor() const
{
    return m_Color;
}

void ColorWheel::setColor(const QColor &in_Color)
{
    if(m_Color != in_Color)
    {
        m_Color = in_Color;
        emit colorChanged(m_Color);
        indicatorUpdate();
        drawChooser();
    }
}

void ColorWheel::setColor(const float in_Hue, const float in_Saturation, const float in_Value, const float in_Alpha)
{
    QColor in_Color(Qt::black);
    in_Color.setHsvF(in_Hue, in_Saturation, in_Value, in_Alpha);
    m_Color = in_Color;
    emit colorChanged(m_Color);
    indicatorUpdate();
    chooserUpdate();
}

void ColorWheel::setHue(const float in_Hue)
{
    QColor tmpColor(Qt::black);
    tmpColor.setHsvF(in_Hue, m_Color.saturationF(), m_Color.valueF(), m_Color.alphaF());
    m_Color = tmpColor;
    emit colorChanged(m_Color);
}

void ColorWheel::setSaturation(const float in_Saturation)
{
    QColor in_Color(Qt::black);

    in_Color.setHsvF(m_Color.hueF(), in_Saturation, m_Color.valueF(), m_Color.alphaF());
    m_Color = in_Color;
    emit colorChanged(m_Color);
}
void ColorWheel::setValue(const float in_Value)
{
    QColor tmpColor(Qt::black);
    tmpColor.setHsvF(m_Color.hueF(), m_Color.saturationF(), in_Value, m_Color.alphaF());
    m_Color = tmpColor;
    emit colorChanged(m_Color);
}

void ColorWheel::setAlpha(const float in_Alpha)
{
    QColor tmpColor(Qt::black);
    tmpColor.setHsvF(m_Color.hueF(), m_Color.saturationF(), m_Color.valueF(), in_Alpha);
    m_Color = tmpColor;
    emit colorChanged(m_Color);
}

void ColorWheel::initialize()
{
    // Wheel gradient
    m_WheelGradient.setAngle(0.0);
    m_WheelGradient.setCenter(QPointF(0.0, 0.0));
    m_WheelGradient.setColorAt(0.0, Qt::red);
    m_WheelGradient.setColorAt(60.0 /360.0, Qt::yellow);
    m_WheelGradient.setColorAt(120.0 /360.0, Qt::green);
    m_WheelGradient.setColorAt(180.0 /360.0, Qt::cyan);
    m_WheelGradient.setColorAt(240.0 /360.0, Qt::blue);
    m_WheelGradient.setColorAt(300.0 /360.0, Qt::magenta);
    m_WheelGradient.setColorAt(1.0, Qt::red);

    // Sampler gradient
    m_ValueGradient.setColorAt(0.0, Qt::transparent);
    m_ValueGradient.setColorAt(1.0, Qt::black);

    // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX   REMOVER XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    countWheel = 1;
    countChooser = 1;
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
        setHue((calcAngle(m_MouseVec.normalize(), Physis::PhVector3(1.0f,0.0f,0.0f).normalize())) / 360);
    }
}

 void ColorWheel::chooserUpdate()
 {
     if(m_QuadHit)
     {
         float x;
         float y;

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
             setSaturation(0.5f);
         }
         else
         {
             setSaturation((x / m_ChooserSize.width()) + 0.5f);
         }


         // Setup value
         if (y == 0)
         {
             setValue(0.5);
         }
         else
         {
             setValue( (y / m_ChooserSize.height()) + 0.5f);
         }
     }

     indicatorUpdate();
 }

void ColorWheel::indicatorUpdate()
{
    if(getColor().saturationF() == 0.5f)
    {
        m_IndicatorPosition.setX(0.0f);
    }
    else
    {
        m_IndicatorPosition.setX(m_Color.saturationF() * m_ChooserSize.width() - (m_ChooserSize.width() / 2));
    }

    if(getColor().valueF() == 0.5f)
    {
        m_IndicatorPosition.setY(0.0f);
    }
    else
    {
        m_IndicatorPosition.setY(-(m_Color.valueF() * m_ChooserSize.height() - (m_ChooserSize.height() / 2)));
    }
    update();
}

void ColorWheel::drawWheel()
{
    m_WheelPixmap = QPixmap(width(), height());
    m_WheelPixmap.fill(QColor(0,0,0,0));

    QPainterPath path;
    QPainter painter(&m_WheelPixmap);
    painter.translate(m_WorldCenter);
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_WheelGradient);
    path.addEllipse(QPointF(0.0f, 0.0f), m_InnerRadius, m_InnerRadius);
    path.addEllipse(QPointF(0.0f, 0.0f), m_OuterRadius, m_OuterRadius);
    painter.drawPath(path);

    // Draw chooser foreground
    painter.setRenderHints(QPainter::Antialiasing, false);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    painter.setBrush(m_ValueGradient);
    painter.drawRect(m_ChooserSize);

    qDebug() << "Desenha wheel" << countWheel++;
}

void ColorWheel::drawChooser()
{
    m_ChooserPixmap = QPixmap(width(), height());
    m_ChooserPixmap.fill(QColor(0,0,0,0));

    QPainter painter(&m_ChooserPixmap);
    QColor pureColor;
    pureColor.setHsvF(getColor().hueF(), 0.0f, 1.0f, 1.0f);
    QColor valueColor(Qt::black);
    valueColor.setHsvF(getColor().hueF(), 1.0f, 1.0f, 1.0f);

    painter.translate(m_WorldCenter);
    painter.setRenderHints(QPainter::Antialiasing, false);
    m_SaturationGradient.setColorAt(1.0f, valueColor);
    m_SaturationGradient.setColorAt(0.0f, pureColor);

    painter.setPen(QPen(Qt::black, 1, Qt::NoPen));
    painter.setBrush(m_SaturationGradient);
    painter.drawRect(m_ChooserSize);

    update();

    qDebug() << "Desenha chooser" << countChooser++;

}

void ColorWheel::drawIndicators(QPainter *painter)
{
    painter->setRenderHints(QPainter::Antialiasing, false);
    painter->setPen(QPen(Qt::black,1, Qt::SolidLine));

    // Rotate Indicator
    painter->save();
    painter->rotate(-getColor().hueF() * 360);

    // Draw wheel indicator arrow
    painter->drawLine(QPointF(m_InnerRadius + 1.0f, 0.0f), QPointF(m_OuterRadius, 0.0f));
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::black);
    painter->translate(m_InnerRadius, 0.0f);
    painter->drawConvexPolygon(m_Arrow, 3);
    painter->restore();

    // Draw chooser indicator
    painter->setBrush(m_Color);
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    painter->drawEllipse(m_IndicatorPosition, m_IndicatorSize, m_IndicatorSize);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(Qt::white, 1, Qt::SolidLine));
    painter->drawEllipse(m_IndicatorPosition, m_IndicatorSize -1.0 , m_IndicatorSize - 1.0);
}


void ColorWheel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.translate(m_WorldCenter);
    painter.drawPixmap(-width()/2, -height()/2, m_ChooserPixmap);
    painter.drawPixmap(-width()/2, -height()/2, m_WheelPixmap);
    drawIndicators(&painter);
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
      drawChooser();
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

//    if(!m_QuadHit && m_WheelHit)
//    {
//        drawChooser();
//    }
}

void ColorWheel::mouseMoveEvent(QMouseEvent *event)
{
    m_MouseVec.x = event->x() - width() / 2.0f;
    m_MouseVec.y = event->y() - height() / 2.0f;

    if(m_WheelHit)
    {
        wheelUpdate();
        drawChooser();
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
    m_WorldCenter.setX(width() / 2);
    m_WorldCenter.setY(height() / 2);

    float side = std::fmin(width(), height()) / 2;
    m_OuterRadius = side - 1.0f;
    m_InnerRadius = side - (side * 0.25f);

    // Make the arrow points relative to widget size
   m_Arrow [0] = QPoint(0, 3 * (side * 0.01));
   m_Arrow [1] = QPoint(6 * (side * 0.01), 0);
   m_Arrow [2] = QPoint(0, -3 * (side * 0.01));

   // Calculate the size for sampler color
   float diagonal = std::cos(physis::math::degreeToRadians(45.0f)) * m_InnerRadius;
   float gap = (diagonal * 0.05f);

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

   drawWheel();
   drawChooser();
   indicatorUpdate();
}
