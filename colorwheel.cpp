#include "colorwheel.h"

ColorWheel::ColorWheel(QWidget *parent) :
    QWidget(parent),
    m_Color(Qt::red)
{
    initialize();
}

ColorWheel::ColorWheel(const QColor in_Color, QWidget *parent) :
    QWidget(parent),
    m_Color(in_Color)
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
    m_Color = in_Color;
    emit colorChanged(m_Color);
    indicatorUpdate();
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

double ColorWheel::angleAt(Physis::PhVector3 v1, Physis::PhVector3 v2)
{
    double angle = physis::math::radiansToDegrees(std::acos( v1.dot(v2) / (v1.length() * v2.length() ) ));

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
        m_hue = angleAt(m_MouseVec.normalize(), Physis::PhVector3(1.0, 0.0, 0.0).normalize()) / 360;
        QColor tmp;
        tmp.setHsvF(m_hue, m_Color.saturationF(), m_Color.valueF());
        setColor(tmp);
        update();
    }
}

 void ColorWheel::chooserUpdate()
 {
     if(m_ChooserHit)
     {
         double x;
         double y;

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
             m_saturation = 0.5;
         }
         else
         {
             m_saturation = (x / m_ChooserSize.width()) + 0.5;
         }


         // Setup value
         if (y == 0)
         {
             m_value = 0.5;
         }
         else
         {
             m_value = (y / m_ChooserSize.height()) + 0.5;
         }
     }

     QColor tmp;
     tmp.setHsvF(m_Color.hslHueF(), m_saturation, m_value);
     setColor(tmp);
     indicatorUpdate();
 }

void ColorWheel::indicatorUpdate()
{
    if(getColor().saturationF() == 0.5)
    {
        m_IndicatorPosition.setX(0.0);
    }
    else
    {
        m_IndicatorPosition.setX(m_Color.saturationF() * m_ChooserSize.width() - (m_ChooserSize.width() / 2));
    }

    if(getColor().valueF() == 0.5)
    {
        m_IndicatorPosition.setY(0.0);
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
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.translate(m_WorldCenter.x, m_WorldCenter.y);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_WheelGradient);
    path.addEllipse(QPointF(0, 0), m_InnerRadius, m_InnerRadius);
    path.addEllipse(QPointF(0, 0), m_OuterRadius, m_OuterRadius);
    painter.drawPath(path);

    // Draw chooser foreground
    painter.setRenderHints(QPainter::Antialiasing, false);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    painter.setBrush(m_ValueGradient);
    painter.drawRect(m_ChooserSize);
}

void ColorWheel::drawChooser()
{
    m_ChooserPixmap = QPixmap(m_ChooserSize.width(), m_ChooserSize.height());
    m_ChooserPixmap.fill(QColor(0,0,0,0));

    QColor pureColor;
    pureColor.setHsvF(m_hue, 1.0, 1.0, 1.0);

    m_SaturationGradient.setColorAt(1.0, pureColor);
    m_SaturationGradient.setColorAt(0, Qt::white);

    QPainter painter(&m_ChooserPixmap);
    painter.setRenderHints(QPainter::Antialiasing, false);
    painter.setPen(QPen(Qt::black, 1, Qt::NoPen));
    painter.setBrush(m_SaturationGradient);
    painter.drawRect(0, 0, m_ChooserSize.width(), m_ChooserSize.height());
}

void ColorWheel::drawIndicators()
{
    qDebug() << "Variável: " << m_saturation;
    qDebug() << "Cor:      " << m_Color.saturationF();

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.translate(m_WorldCenter.x, m_WorldCenter.y);

    // Rotate Indicator
    painter.save();
    painter.rotate(-m_hue * 360);


    // Draw wheel indicator
    painter.setBrush(Qt::black);
    painter.drawConvexPolygon(m_Arrow, 3);
    painter.drawLine(QPointF(m_InnerRadius + 1.0, 0), QPointF(m_OuterRadius, 0));
    painter.restore();

    // Draw chooser indicator
    painter.drawEllipse(m_IndicatorPosition, m_IndicatorSize, m_IndicatorSize);
    painter.setBrush(m_Color);
    painter.setPen(QPen(Qt::white, 1, Qt::SolidLine));
    painter.drawEllipse(m_IndicatorPosition, m_IndicatorSize - 1.0, m_IndicatorSize - 1.0);
}

void ColorWheel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(width()/2 - (m_ChooserSize.width()/2), height()/2 - (m_ChooserSize.height()/2), m_ChooserPixmap);
    painter.drawPixmap(0, 0, m_WheelPixmap);
    drawIndicators();
}

void ColorWheel::mousePressEvent(QMouseEvent *event)
{    
    if(event->buttons() != Qt::LeftButton)
    {
        return;
    }

    m_MouseVec.x = event->x() - width() / 2;
    m_MouseVec.y = event->y() - height() / 2;

    m_WheelHit = isWheelHit();
    m_ChooserHit = isQuadHit();

    if(m_WheelHit)
    {
        wheelUpdate();
        drawChooser();
        return;
    }

    if(m_ChooserHit)
    {
        setCursor(Qt::BlankCursor);
        chooserUpdate();
        return;
    }
}

void ColorWheel::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_WheelHit = false;
    m_ChooserHit = false;
    setCursor(Qt::ArrowCursor);
}

void ColorWheel::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() != Qt::LeftButton)
    {
        return;
    }

    m_MouseVec.x = event->x() - width() / 2;
    m_MouseVec.y = event->y() - height() / 2;

    if(m_WheelHit)
    {
        wheelUpdate();
        drawChooser();
        return;
    }

    if(m_ChooserHit)
    {
        chooserUpdate();
        return;
    }
}

void ColorWheel::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    // Move world origin to center of screen
    m_WorldCenter.x = width() / 2;
    m_WorldCenter.y = height() / 2;

    double side = std::fmin(width(), height()) / 2;
    m_OuterRadius = side - 1.0;
    m_InnerRadius = side - (side * 0.25);

    // Make the arrow points relative to widget size
   m_Arrow [0] = QPointF(m_InnerRadius, 3.0 * (side * 0.01));
   m_Arrow [1] = QPointF(m_InnerRadius + 6.0 * (side * 0.01), 0);
   m_Arrow [2] = QPointF(m_InnerRadius, -3.0 * (side * 0.01));

   // Calculate the size for sampler color
   double diagonal = std::cos(physis::math::degreeToRadians(45.0)) * m_InnerRadius;
   double gap = (diagonal * 0.05);

   // Calculate chooser indicator size
   m_IndicatorSize = m_InnerRadius * 0.04;
   (m_IndicatorSize < 1.0) ? m_IndicatorSize = 1.0 : m_IndicatorSize;

   // Setup the rect size for color sampler
   m_ChooserSize.setTopLeft(QPointF( -diagonal + gap, -diagonal + gap ));
   m_ChooserSize.setBottomRight(QPointF( diagonal - gap, diagonal - gap));

   m_SaturationGradient.setStart(QPointF(0, 0 ));
   m_SaturationGradient.setFinalStop(QPointF(m_ChooserSize.width(), 0));

   m_ValueGradient.setStart(QPointF(0, -diagonal + gap ));
   m_ValueGradient.setFinalStop(QPointF(0, diagonal - gap));

   drawWheel();
   drawChooser();
   indicatorUpdate();
}
