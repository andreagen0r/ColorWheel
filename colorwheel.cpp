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
    connect(this, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)));
    this->setMinimumSize(150,150);
    this->setMaximumSize(600,600);

    // Setup gradient position and color
    mGradient.setAngle(0);
    mGradient.setCenter(QPointF(0,0));
    mGradient.setColorAt(0, Qt::red);
    mGradient.setColorAt(60.0/360.0, Qt::yellow);
    mGradient.setColorAt(120.0/360.0, Qt::green);
    mGradient.setColorAt(180.0/360.0, Qt::cyan);
    mGradient.setColorAt(240.0/360.0, Qt::blue);
    mGradient.setColorAt(300.0/360.0, Qt::magenta);
    mGradient.setColorAt(1, Qt::red);
}

QColor ColorWheel::getColor() const
{
    return this->mColor;
}

void ColorWheel::setColor(const QColor &inValue)
{
    this->mColor = inValue;
}

float ColorWheel::getHue() const
{
    return this->mHue;
}

void ColorWheel::setHue(const float &inValue)
{
    this->mHue = inValue;
}

void ColorWheel::changeColor()
{
    QColor c(getColor());
    c.setHsvF( -(getHue()/360), 1.0, 1.0, 1.0);
    emit colorChanged(c);
}



bool ColorWheel::isHit()
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

float ColorWheel::getAngle(QVector2D v1, QVector2D v2)
{
    double angle = std::acos( v1.dotProduct(v1,v2) / (v1.length() * v2.length() ) ) * (180 / M_PI);

    if(mouseQuadTest == LEFT_DOWN || mouseQuadTest == RIGHT_DOWN)
        angle = 360 - angle;

    return angle;
}

void ColorWheel::mouseCalc()
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
    QPainterPath path;
    painter.setRenderHints(QPainter::Antialiasing);

    painter.translate(this->width() / 2, this->height() / 2);

    // Draw cartesian coordinates center
//    painter.setPen(QPen(Qt::gray,1));
//    painter.drawLine(-(this->width()), 0, this->width(), 0);
//    painter.drawLine(0, -(this->height()), 0, this->height());

    // Draw wheel
    painter.setPen(Qt::NoPen);
    painter.setBrush(mGradient);
    path.addEllipse(QPointF(0,0), mOuterRadius, mOuterRadius);
    path.addEllipse(QPointF(0,0), mInnerRadius, mInnerRadius);
    painter.drawPath(path);

    // Draw color indicator
    painter.save();
    painter.setClipPath(path);
    painter.setPen(QPen(Qt::black,1, Qt::SolidLine));

    // Rotate Indicator
    painter.rotate(getHue());
    qDebug() << getHue();
    qDebug() << mColor.hueF() * 360;

    // Draw color indicator line
    painter.drawLine(QPointF(mInnerRadius,0), QPointF(mOuterRadius, 0));

    // Draw color indicator arrow
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.translate(mInnerRadius, 0);
    painter.drawConvexPolygon(arrow, 3);
    painter.setClipping(false);
    painter.restore();

    // Draw debug vector line from mouse position
//    painter.save();
//    painter.setPen(QPen(Qt::white,2, Qt::DotLine));
//    QPointF m = this->mapFromGlobal(QCursor::pos());
//    painter.drawLine(QPointF(0,0),QPointF(m.x() - this->width()/2, m.y() - this->height()/2));
//    painter.restore();

    // Sample color
    painter.save();
    painter.setBrush(getColor());

    // versao antiga usando QtMath
    // double diagonal = cos(qDegreesToRadians(45.0)) * mInnerRadius;

    // Versão nova usando STL cmath
    float diagonal = std::cos(45.0 * (M_PI / 180)) * mInnerRadius;
    //    // Converts degrees to radians.
    //    #define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)

    //    // Converts radians to degrees.
    //    #define radiansToDegrees(angleRadians) (angleRadians * 180.0 / M_PI)

    double gap = (diagonal * 0.01);
    painter.drawRect(QRect(QPoint( -diagonal + gap, -diagonal + gap ), QPoint( diagonal - gap, diagonal - gap) ) );
    painter.restore();
}

void ColorWheel::mousePressEvent(QMouseEvent *event)
{
    mMouseVec.setX(event->x() - this->width() / 2);
    mMouseVec.setY(-(event->y() - this->height() / 2));

    mouseCalc();

    mWheelHit = isHit();

    if(isHit())
    {
        setHue( - (getAngle(mMouseVec.normalized(), QVector2D(1,0).normalized())) );
        changeColor();
    }

    repaint();
}

void ColorWheel::mouseMoveEvent(QMouseEvent *event)
{
    mMouseVec.setX(event->x() - this->width() / 2);
    mMouseVec.setY(-(event->y() - this->height() / 2));
    mouseCalc();

    if(mWheelHit)
    {
        setHue( - (getAngle(mMouseVec.normalized(), QVector2D(1,0).normalized())) );
        changeColor();
    }

    repaint();
}

void ColorWheel::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

//    int side = qMin(this->width(), this->height()) / 2;
    int side = std::min(this->width(), this->height()) / 2;
    mOuterRadius = side - 1;
    mInnerRadius = side - (side * 0.25);

    // Make the arrow points relative to widget size
   arrow [0] = QPoint(0, 3 * (side * 0.01));
   arrow [1] = QPoint(6 * (side * 0.01), 0);
   arrow [2] = QPoint(0, -3 * (side * 0.01));
}
