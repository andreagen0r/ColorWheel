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

void ColorWheel::setColor(const QColor &inColor)
{
    this->mColor = inColor;
    emit colorChanged(this->mColor);
    indicatorUpdate();
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
    if(mColorSampler.left() <= mMouseVec.x() && mColorSampler.right() >= mMouseVec.x()
            && mColorSampler.top() <= mMouseVec.y() && mColorSampler.bottom() >= mMouseVec.y())
    {
        return true;
    }
    else
    {
        return false;
    }
}

float ColorWheel::calcAngle(QVector2D v1, QVector2D v2)
{
    float angle = physis::math::radiansToDegrees(std::acos( v1.dotProduct(v1,v2) / (v1.length() * v2.length() ) ));

    if(getQuadrant() == LEFT_DOWN || getQuadrant() == RIGHT_DOWN)
        angle = 360 - angle;

    return angle;
}

short ColorWheel::getQuadrant()
{
    QPoint mousePosition = this->mapFromGlobal(QCursor::pos());

    if(mousePosition.x() < this->width()/2)
    {
        if(mousePosition.y() < this->height()/2)
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
        if(mousePosition.y() < this->height()/2)
        {
            return RIGHT_UP;
        }
        else
        {
            return RIGHT_DOWN;
        }
    }
}

void ColorWheel::drawWheel()
{
    QPainter painter(this);
    QPainterPath path, border;
    painter.setRenderHints(QPainter::Antialiasing, true);

    // Move the world to the center of the screen
    painter.translate(mWorldCenter);

    // Draw wheel
    painter.setPen(Qt::NoPen);
    painter.setBrush(mGradient);
    path.addEllipse(QPointF(0,0), mInnerRadius, mInnerRadius);
    path.addEllipse(QPointF(0,0), mOuterRadius, mOuterRadius);
    painter.drawPath(path);
}

void ColorWheel::drawIndicators()
{
    QPainter painter(this);
    QPainterPath path, border;
    painter.setRenderHints(QPainter::Antialiasing, true);

    // Move the world to the center of the screen
    painter.translate(mWorldCenter);

    // Draw color indicator
    painter.save();
//    painter.setClipPath(path);
    painter.setPen(QPen(Qt::black,1, Qt::SolidLine));

    // Rotate Indicator
    painter.rotate(-getColor().hueF() * 360);

    // temporario XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX deletar depois XXXXXXXXXXXXXXXXXXXXXX
    painter.setBrush(getColor());
    painter.drawRect(mOuterRadius, -20, 40,40);
    painter.drawEllipse(QPointF(mInnerRadius + (mOuterRadius-mInnerRadius)/2 ,0), (mOuterRadius-mInnerRadius)/2, (mOuterRadius-mInnerRadius)/2);
    // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    // Draw color indicator line
    painter.drawLine(QPointF(mInnerRadius,0), QPointF(mOuterRadius, 0));

    // Draw color indicator arrow
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.translate(mInnerRadius, 0);
    painter.drawConvexPolygon(arrow, 3);
    painter.setClipping(false);
    painter.restore();


    // Color Sampler indicator
    painter.setBrush(Qt::white);

    int indicatorBorder;
    (std::min(this->width(), this->height()) < 500) ? indicatorBorder = 1 : indicatorBorder = 2;

    painter.setPen(QPen(Qt::black, indicatorBorder, Qt::SolidLine));
    painter.save();
    painter.drawEllipse(mIndicatorPosition, static_cast<int>(mInnerRadius * 0.05), static_cast<int>(mInnerRadius * 0.05));
    painter.restore();

}

void ColorWheel::indicatorUpdate()
{
    float sat, val;
    sat = getColor().saturationF();
    val = getColor().valueF();

    if(sat == 0.5)
    {
        mIndicatorPosition.setX(0);
    }
    else
    {
        if(sat > 0.5)
        {
            sat = sat * mColorSampler.width() - mColorSampler.width()/2;
            mIndicatorPosition.setX(sat);
        }
        if(sat < 0.5)
        {
            sat = sat * mColorSampler.width() - mColorSampler.width()/2;
            mIndicatorPosition.setX(sat);
        }
    }


    if(val == 0.5)
    {
        mIndicatorPosition.setY(0);
    }
    else
    {
        if(val > 0.5)
        {
            val = val * mColorSampler.height() - mColorSampler.height()/2;
            mIndicatorPosition.setY(-val);
        }
        if(val < 0.5)
        {
            val = val * mColorSampler.height() - mColorSampler.height()/2;
            mIndicatorPosition.setY(-val);
        }
    }

}

void ColorWheel::setHue()
{
    if(mWheelHit)
    {
        //setHue( - (getAngle(mMouseVec.normalized(), QVector2D(1,0).normalized())) );
        QColor c;
        c.setHsvF( (calcAngle(mMouseVec.normalized(), QVector2D(1,0).normalized())) / 360, getColor().saturationF(),
                  getColor().valueF(), getColor().alphaF());
        setColor(c);
    }
}

void ColorWheel::setSaturationValue()
{
    int x, y;
    x = mMouseVec.x();
    y = - mMouseVec.y();
    if(mQuadHit)
    {
        if(mMouseVec.x() < mColorSampler.left())
        {
            x = mColorSampler.left();
        }

        if(mMouseVec.x() > mColorSampler.right())
        {
            x = mColorSampler.right();
        }

        if(mMouseVec.y() < mColorSampler.top())
        {
            y = mColorSampler.bottom();
        }

        if(mMouseVec.y() > mColorSampler.bottom())
        {
            y = mColorSampler.top();
        }


        float vx, vy = 0;
        QColor cc(getColor());
        if (x == 0)
        {
            cc.setHsvF(getColor().hueF(), 0.5, getColor().valueF(), getColor().alphaF());
            setColor(cc);
        }
        else
        {
            vx = (x / mColorSampler.width()) + 0.5;
            cc.setHsvF(getColor().hueF(), vx, getColor().valueF(), getColor().alphaF());
            setColor(cc);
        }

        if (y == 0)
        {
            cc.setHsvF(getColor().hueF(), getColor().saturationF(), 0.5, getColor().alphaF());
            setColor(cc);
        }
        else
        {
            vy = (y / mColorSampler.height()) + 0.5;
            cc.setHsvF(getColor().hueF(), getColor().saturationF(), vy, getColor().alphaF());
            setColor(cc);
        }

    }
}

void ColorWheel::drawSaturationValue()
{
    QPainter painter(this);
    // Move the world to the center of the screen
    painter.translate(mWorldCenter);

    QColor saturatedColor;
    saturatedColor.setHsvF(getColor().hueF(), 0, 1.0, 1.0);

    QColor cc(Qt::black);
    cc.setHsvF(getColor().hueF(), 1.0,1.0,1.0);
    saturationGradient.setColorAt(1.0, cc);
    saturationGradient.setColorAt(0.0, saturatedColor);

    painter.setBrush(saturationGradient);
    painter.drawRect(mColorSampler);
    painter.setBrush(valueGradient);
    painter.drawRect(mColorSampler );
}

void ColorWheel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    drawWheel();
    drawSaturationValue();
    drawIndicators();
}

void ColorWheel::mousePressEvent(QMouseEvent *event)
{
    mMouseVec.setX(event->x() - this->width() / 2);
    mMouseVec.setY(event->y() - this->height() / 2);

    getQuadrant();

    mWheelHit = isWheelHit();
    mQuadHit = isQuadHit();

    setHue();
    setSaturationValue();

    repaint();
}

void ColorWheel::mouseMoveEvent(QMouseEvent *event)
{
    mMouseVec.setX(event->x() - this->width() / 2);
    mMouseVec.setY(event->y() - this->height() / 2);
    getQuadrant();

    setHue();
    setSaturationValue();

    repaint();
}

void ColorWheel::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    // Move world origin to center of screen
    mWorldCenter.setX(this->width() / 2);
    mWorldCenter.setY(this->height() / 2);

    int side = std::min(this->width(), this->height()) / 2;
    mOuterRadius = side - 1;
    mInnerRadius = side - (side * 0.25);

    // Make the arrow points relative to widget size
   arrow [0] = QPoint(0, 3 * (side * 0.01));
   arrow [1] = QPoint(6 * (side * 0.01), 0);
   arrow [2] = QPoint(0, -3 * (side * 0.01));

   // Calculate the size for sampler color
   float diagonal = std::cos(physis::math::degreeToRadians(45.0)) * mInnerRadius;
   float gap = (diagonal * 0.05);

   // Setup the rect size for color sampler
   mColorSampler.setTopLeft(QPoint( -diagonal + gap, -diagonal + gap ));
   mColorSampler.setBottomRight(QPoint( diagonal - gap, diagonal - gap));

   saturationGradient.setStart(QPointF(-diagonal + gap, 0 ));
   saturationGradient.setFinalStop(QPointF(diagonal - gap, 0));

   valueGradient.setStart(QPointF(0, -diagonal + gap ));
   valueGradient.setFinalStop(QPointF(0, diagonal - gap));

   indicatorUpdate();
}
