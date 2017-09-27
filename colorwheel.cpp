#include "colorwheel.h"


// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX   DELETAR XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
static int globalCount = 0;
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX   DELETAR XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

ColorWheel::ColorWheel(QWidget *parent)
    : QWidget(parent), m_hitMode(HitPosition::IDLE), m_Color(Qt::red)
{
    initialize();
}

ColorWheel::ColorWheel(const QColor in_Color, QWidget *parent)
    : QWidget(parent), m_hitMode(HitPosition::IDLE), m_Color(in_Color)
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
    if(in_Color != m_Color)
    {
        isChooserRefreshed = (m_Color.hsvSaturationF() != in_Color.hsvSaturationF() || m_Color.valueF() != in_Color.valueF());
        isColorRefreshed = (m_Color.hsvHueF() != in_Color.hsvHueF());

        m_Color = in_Color;
        emit colorChanged(m_Color);
        if(isChooserRefreshed)
        {
            chooserIndicator();
            isChooserRefreshed = false;
        }

        if(isColorRefreshed)
        {
            drawColorChooser();
            isColorRefreshed = false;
        }
        update();
    }
}

void ColorWheel::initialize()
{    
    // Wheel gradient
    m_wheelGradient.setAngle(0.0);
    m_wheelGradient.setCenter(QPointF(0.0, 0.0));
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

bool ColorWheel::isHitMode()
{    
    if(m_mouseVec.length() > m_innerRadius && m_mouseVec.length() < m_outerRadius)
    {
        m_hitMode = HitPosition::WHEEL;
        return true;
    }

    if(m_chooserSize.contains(m_mouseVec.x, m_mouseVec.y))
    {
        m_hitMode = HitPosition::CHOOSER;
        return true;
    }

    return false;
}

auto ColorWheel::getQuadrant()
{
    QPoint mousePosition(mapFromGlobal(QCursor::pos()));

    if(mousePosition.x() <= m_width/2)
    {
        if(mousePosition.y() <= m_height/2)
        {
            return Quadrant::LEFT_UP;
        }
        else
        {
           return Quadrant::LEFT_DOWN;
        }
    }
    else
    {
        if(mousePosition.y() <= m_height/2)
        {
            return Quadrant::RIGHT_UP;
        }
        else
        {            
            return Quadrant::RIGHT_DOWN;
        }
    }
}

double ColorWheel::angleAt(Physis::PhVector3 in_vec1, Physis::PhVector3 in_vec2)
{
    double angle = physis::math::radiansToDegrees(std::acos(in_vec1.dot(in_vec2) / (in_vec1.length() * in_vec2.length())));

    m_quadHit = getQuadrant();

    if(m_quadHit == Quadrant::LEFT_DOWN || m_quadHit == Quadrant::RIGHT_DOWN)
    {
        return angle = 360 - angle;
    }

    return angle;
}

void ColorWheel::chooserRefresh()
{
    double x;
    double y;

    // Test Saturation X axis
    if(m_mouseVec.x <= m_chooserSize.left())
    {
        x = m_chooserSize.left();
    }
    else if (m_mouseVec.x >= m_chooserSize.right())
    {
        x = m_chooserSize.right();
    }
    else
    {
        x = m_mouseVec.x;
    }

    // Test Value Y axis
    if(m_mouseVec.y <= m_chooserSize.top())
    {
        y = -m_chooserSize.top();
    }
    else if (m_mouseVec.y >= m_chooserSize.bottom())
    {
        y = -m_chooserSize.bottom();
    }
    else
    {
        y = -m_mouseVec.y;
    }

    double m_sat = (x / m_chooserSize.width()) + 0.5;
    double m_val = (y / m_chooserSize.height()) + 0.5;

    QColor color;
    color.setHsvF(m_Color.hsvHueF(), m_sat, m_val);
    setColor(color);
}

void ColorWheel::chooserIndicator()
{
    if(m_Color.hsvSaturationF() == 0.5)
    {
        m_indicatorPosition.setX(0.0);
    }
    else
    {
        m_indicatorPosition.setX(m_Color.saturationF() * m_chooserSize.width() - (m_chooserSize.width() / 2));
    }

    if(m_Color.valueF() == 0.5)
    {
        m_indicatorPosition.setY(0.0);
    }
    else
    {
        m_indicatorPosition.setY(-(m_Color.valueF() * m_chooserSize.height() - (m_chooserSize.height() / 2)));
    }
}

void ColorWheel::wheelRefresh()
{
    double m_hue = angleAt(m_mouseVec.normalize(), Physis::PhVector3(1.0, 0.0, 0.0).normalize()) / 360;
    QColor color;
    color.setHsvF(m_hue, m_Color.hsvSaturationF(), m_Color.valueF());
    setColor(color);
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
    path.addEllipse(QPointF(0, 0), m_innerRadius, m_innerRadius);
    path.addEllipse(QPointF(0, 0), m_outerRadius, m_outerRadius);
    painter.drawPath(path);

    // Draw chooser foreground
    painter.setRenderHints(QPainter::Antialiasing, false);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    painter.setBrush(m_valueGradient);
    painter.drawRect(m_chooserSize);
}

void ColorWheel::drawColorChooser()
{
    m_chooserPixmap = QPixmap(m_chooserSize.width(), m_chooserSize.height());
    m_chooserPixmap.fill(QColor(0,0,0,0));

    QColor pureColor;
    pureColor.setHsvF(m_Color.hsvHueF(), 1.0, 1.0, 1.0);

    m_saturationGradient.setColorAt(1.0, pureColor);
    m_saturationGradient.setColorAt(0, Qt::white);

    QPainter p(&m_chooserPixmap);
    p.setRenderHints(QPainter::Antialiasing, false);
    p.setPen(QPen(Qt::black, 1, Qt::NoPen));
    p.setBrush(m_saturationGradient);
    p.drawRect(0, 0, m_chooserSize.width(), m_chooserSize.height());
    isChooserRefreshed = false;
}

void ColorWheel::drawIndicators(QPainter *painter)
{    
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->translate(m_worldCenter.x(), m_worldCenter.y());

    // Rotate Indicator
    painter->save();
    painter->rotate(-m_Color.hsvHueF() * 360.0);

    // Draw wheel indicator
    painter->setBrush(Qt::black);
    painter->drawConvexPolygon(m_arrow, 3);
    painter->drawLine(QPointF(m_innerRadius + 1.0, 0), QPointF(m_outerRadius, 0));
    painter->restore();

    // Draw chooser indicator
    painter->drawEllipse(m_indicatorPosition, m_indicatorSize, m_indicatorSize);
    painter->setBrush(m_Color);
    painter->setPen(QPen(Qt::white, 1, Qt::SolidLine));
    painter->drawEllipse(m_indicatorPosition, m_indicatorSize - 1.0, m_indicatorSize - 1.0);
}

void ColorWheel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(m_width/2 - (m_chooserSize.width()/2), m_height/2 - (m_chooserSize.height()/2), m_chooserPixmap);
    painter.drawPixmap(0, 0, m_wheelPixmap);
    drawIndicators(&painter);
}

void ColorWheel::mousePressEvent(QMouseEvent *event)
{    
    if(event->buttons() != Qt::LeftButton)
    {
        return;
    }

    m_mouseVec.x = event->x() - m_width / 2;
    m_mouseVec.y = event->y() - m_height / 2;

    if(!isHitMode())
    {
        return;
    }

    switch (m_hitMode)
    {
        case HitPosition::WHEEL:
        {
            wheelRefresh();
            break;
        }
        case HitPosition::CHOOSER:
        {
            isChooserRefreshed = true;
            chooserRefresh();
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

    m_mouseVec.x = event->x() - m_width / 2;
    m_mouseVec.y = event->y() - m_height / 2;

    switch (m_hitMode)
    {
        case HitPosition::WHEEL:
        {
            wheelRefresh();
            break;
        }
        case HitPosition::CHOOSER:
        {
            isChooserRefreshed = true;
            chooserRefresh();
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
    m_worldCenter.setX(m_width / 2);
    m_worldCenter.setY(m_height / 2);

    double side = std::fmin(width(), height()) / 2;
    m_outerRadius = side - 1.0;
    m_innerRadius = side - (side * 0.25);

    // Make the arrow points relative to widget size
   m_arrow [0] = QPoint(m_innerRadius, 3 * (side * 0.01));
   m_arrow [1] = QPoint(m_innerRadius + 6 * (side * 0.01), 0);
   m_arrow [2] = QPoint(m_innerRadius, -3 * (side * 0.01));

   // Calculate the size for sampler color
   double diagonal = std::cos(physis::math::degreeToRadians(45.0)) * m_innerRadius;
   double gap = (diagonal * 0.05);

   // Calculate chooser indicator size
   m_indicatorSize = m_innerRadius * 0.04;
   if(m_indicatorSize < 1.0)
   {
       m_indicatorSize = 1.0;
   }

   // Setup the rect size for color sampler
   m_chooserSize.setTopLeft(QPointF( -diagonal + gap, -diagonal + gap ));
   m_chooserSize.setBottomRight(QPointF( diagonal - gap, diagonal - gap));

   m_saturationGradient.setStart(QPointF(0, 0 ));
   m_saturationGradient.setFinalStop(QPointF(m_chooserSize.width(), 0));

   m_valueGradient.setStart(QPointF(0, -diagonal + gap ));
   m_valueGradient.setFinalStop(QPointF(0, diagonal - gap));

   drawWheel();
   chooserIndicator();
   drawColorChooser();
}
