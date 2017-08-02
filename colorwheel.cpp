#include "colorwheel.h"

ColorWheel::ColorWheel(QWidget *parent) :
    QWidget(parent)
{
    initialize();
    setColor(Qt::red);
}

ColorWheel::ColorWheel(const QColor &inColor, QWidget *parent) :
    QWidget(parent)
{
    initialize();
    setColor(inColor);
}

ColorWheel::~ColorWheel()
{

}

void ColorWheel::initialize()
{
    // Setup gradient position and color
    mGradient.setAngle(0);
    mGradient.setCenter(QPointF(0,0));
    mGradient.setColorAt(0, Qt::red);
    mGradient.setColorAt(60.0/360.0, Qt::yellow);
    mGradient.setColorAt(120.0/360.0, Qt::green);
    mGradient.setColorAt(180.0/360.0, Qt::cyan);
    mGradient.setColorAt(240.0/360.0, Qt::blue);
    mGradient.setColorAt(300.0/360.0, Qt::magenta);
    mGradient.setColorAt(1.0, Qt::red);

    valueGradient.setColorAt(0.0, QColor(0,0,0,0));
    valueGradient.setColorAt(1.0, Qt::black);
}

QColor ColorWheel::getColor() const
{
    return this->mColor;
}

void ColorWheel::setColor(const QColor &inValue)
{
    this->mColor = inValue;
    emit colorChanged(this->mColor);
}

bool ColorWheel::isWheelHit()
{
    if (mMouseVec.length() > mInnerRadius && mMouseVec.length() < mOuterRadius)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ColorWheel::isQuadHit()
{
    if(colorSampler.left() <= mMouseVec.x() && colorSampler.right() >= mMouseVec.x()
            && colorSampler.top() <= mMouseVec.y() && colorSampler.bottom() >= mMouseVec.y())
    {
        return true;
    }
    else
    {
        return false;
    }
}

float ColorWheel::getAngle(QVector2D v1, QVector2D v2)
{
    float angle = physis::math::radiansToDegrees(std::acos( v1.dotProduct(v1,v2) / (v1.length() * v2.length() ) ));

    if(mouseQuadTest == LEFT_DOWN || mouseQuadTest == RIGHT_DOWN)
        angle = 360 - angle;

    return angle;
}

void ColorWheel::getQuadrant()
{
    QPoint mousePosition = this->mapFromGlobal(QCursor::pos());

    if(mousePosition.x() < this->width()/2)
    {
        if(mousePosition.y() < this->height()/2)
        {
            mouseQuadTest = LEFT_UP;
        }
        else
        {
            mouseQuadTest = LEFT_DOWN;
        }
    }
    else
    {
        if(mousePosition.y() < this->height()/2)
        {
            mouseQuadTest = RIGHT_UP;
        }
        else
        {
            mouseQuadTest = RIGHT_DOWN;
        }
    }
}

void ColorWheel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QPainterPath path, border;
    painter.setRenderHints(QPainter::Antialiasing);

    // Move the world to the center of the screen
    painter.translate(this->width() / 2, this->height() / 2);

    // Draw wheel
    painter.setPen(Qt::NoPen);
    painter.setBrush(mGradient);
    path.addEllipse(QPointF(0,0), mInnerRadius, mInnerRadius);
    path.addEllipse(QPointF(0,0), mOuterRadius, mOuterRadius);
    painter.drawPath(path);

    // Draw color indicator
    painter.save();
    painter.setClipPath(path);
    painter.setPen(QPen(Qt::black,1, Qt::SolidLine));

    // Rotate Indicator
    painter.rotate(- getColor().hueF() * 360);

    // Draw color indicator line
    painter.drawLine(QPointF(mInnerRadius,0), QPointF(mOuterRadius, 0));

    // Draw color indicator arrow
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.translate(mInnerRadius, 0);
    painter.drawConvexPolygon(arrow, 3);
    painter.setClipping(false);
    painter.restore();

    // Sampler color
    painter.save();

    QColor saturatedColor;
    saturatedColor.setHsvF(getColor().hueF(), 0, 1.0, 1.0);

    saturationGradient.setColorAt(1.0, getColor());
    saturationGradient.setColorAt(0.0, saturatedColor);

    painter.setBrush(saturationGradient);
    painter.drawRect(colorSampler);
    painter.setBrush(valueGradient);
    painter.drawRect(colorSampler );
    painter.restore();


    // Sampler Color indicator
    painter.setBrush(Qt::black);
    painter.drawEllipse(QPoint(mMouseVec.x(), - mMouseVec.y()), 10,10);

}

void ColorWheel::mousePressEvent(QMouseEvent *event)
{
    mMouseVec.setX(event->x() - this->width() / 2);
    mMouseVec.setY(-(event->y() - this->height() / 2));

    getQuadrant();

    mWheelHit = isWheelHit();
    mQuadHit = isQuadHit();

    if(isWheelHit())
    {
        //setHue( - (getAngle(mMouseVec.normalized(), QVector2D(1,0).normalized())) );
        QColor c;
        c.setHsvF( (getAngle(mMouseVec.normalized(), QVector2D(1,0).normalized())) / 360, getColor().saturationF(),
                  getColor().valueF(), getColor().alphaF());
        setColor(c);
    }

    repaint();
}

void ColorWheel::mouseMoveEvent(QMouseEvent *event)
{
    mMouseVec.setX(event->x() - this->width() / 2);
    mMouseVec.setY(-(event->y() - this->height() / 2));
    getQuadrant();

    if(mWheelHit)
    {
        QColor c;
        c.setHsvF( (getAngle(mMouseVec.normalized(), QVector2D(1,0).normalized())) / 360, getColor().saturationF(),
                  getColor().valueF(), getColor().alphaF());
        setColor(c);
    }

    if(mQuadHit)
    {
        // fazer a logica do sampler color
    }


    repaint();
}

void ColorWheel::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    int side = std::min(this->width(), this->height()) / 2;
    mOuterRadius = side - 1;
    mInnerRadius = side - (side * 0.25);

    // Make the arrow points relative to widget size
   arrow [0] = QPoint(0, 3 * (side * 0.01));
   arrow [1] = QPoint(6 * (side * 0.01), 0);
   arrow [2] = QPoint(0, -3 * (side * 0.01));

   float diagonal = std::cos(physis::math::degreeToRadians(45.0)) * mInnerRadius;
   float gap = (diagonal * 0.05);

   colorSampler.setTopLeft(QPoint( -diagonal + gap, -diagonal + gap ));
   colorSampler.setBottomRight(QPoint( diagonal - gap, diagonal - gap));

   saturationGradient.setStart(QPointF(-diagonal + gap, 0 ));
   saturationGradient.setFinalStop(QPointF(diagonal - gap, 0));

   valueGradient.setStart(QPointF(0, -diagonal + gap ));
   valueGradient.setFinalStop(QPointF(0, diagonal - gap));

}
