#include "colorwheel.h"

ColorWheel::ColorWheel(QWidget *parent) :
    QWidget(parent)
{
    setColor(Qt::red);
    initialize();
}

ColorWheel::ColorWheel(const QColor &inColor, QWidget *parent) :
    QWidget(parent)
{
    setColor(inColor);
    initialize();
}

ColorWheel::~ColorWheel()
{

}

void ColorWheel::initialize()
{
    // Wheel gradient
    mGradient.setAngle(0.0);
    mGradient.setCenter(QPointF(0.0, 0.0));
    mGradient.setColorAt(0.0, Qt::red);
    mGradient.setColorAt(60.0 /360.0, Qt::yellow);
    mGradient.setColorAt(120.0 /360.0, Qt::green);
    mGradient.setColorAt(180.0 /360.0, Qt::cyan);
    mGradient.setColorAt(240.0 /360.0, Qt::blue);
    mGradient.setColorAt(300.0 /360.0, Qt::magenta);
    mGradient.setColorAt(1.0, Qt::red);

    // Sampler gradient
    valueGradient.setColorAt(0.0, QColor(0,0,0,0));
    valueGradient.setColorAt(1.0, Qt::black);
}

QColor ColorWheel::getColor() const
{
    return mColor;
}

void ColorWheel::setColor(const QColor &inColor)
{
    mColor = inColor;
    emit colorChanged(mColor);
}

void ColorWheel::setColor(const float &inHue, const float &inSaturation, const float &inValue, const float &inAlpha)
{
    QColor inColor(Qt::black);
    inColor.setHsvF(inHue, inSaturation, inValue, inAlpha);
    mColor = inColor;
    emit colorChanged(mColor);
}

void ColorWheel::setHue(const float &inHue)
{
    QColor inColor(Qt::black);
    inColor.setHsvF(inHue, mColor.saturationF(), mColor.valueF(), mColor.alphaF());
    mColor = inColor;
    emit colorChanged(mColor);
}

void ColorWheel::setSaturation(const float &inSaturation)
{
    QColor inColor(Qt::black);
    inColor.setHsvF(mColor.hueF(), inSaturation, mColor.valueF(), mColor.alphaF());
    mColor = inColor;
    emit colorChanged(mColor);
}

void ColorWheel::setValue(const float &inValue)
{
    QColor inColor(Qt::black);
    inColor.setHsvF(mColor.hueF(), mColor.saturationF(), inValue, mColor.alphaF());
    mColor = inColor;
    emit colorChanged(mColor);
}

void ColorWheel::setAlpha(const float &inAlpha)
{
    QColor inColor(Qt::black);
    inColor.setHsvF(mColor.hueF(), mColor.saturationF(), mColor.valueF(), inAlpha);
    mColor = inColor;
    emit colorChanged(mColor);
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
    if(mChooserSize.left() <= mMouseVec.x() && mChooserSize.right() >= mMouseVec.x()
            && mChooserSize.top() <= mMouseVec.y() && mChooserSize.bottom() >= mMouseVec.y())
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
    {
        angle = 360 - angle;
    }

    return angle;
}

short ColorWheel::getQuadrant()
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

 void ColorWheel::chooserUpdate()
 {
     if(mQuadHit)
     {
         int x;
         int y;

         // Test Saturation X axis
         if(mMouseVec.x() < mChooserSize.left())
         {
             x = mChooserSize.left();
         }
         else if (mMouseVec.x() > mChooserSize.right())
         {
             x = mChooserSize.right();
         }
         else
         {
             x = mMouseVec.x();
         }

         // Test Value Y axis
         if(mMouseVec.y() < mChooserSize.top())
         {
             y = -mChooserSize.top();
         }
         else if (mMouseVec.y() > mChooserSize.bottom())
         {
             y = -mChooserSize.bottom();
         }
         else
         {
             y = -mMouseVec.y();
         }



         // Setup saturation
         if (x == 0)
         {
             setSaturation(0.5);
         }
         else
         {
             setSaturation((x / mChooserSize.width()) + 0.5);
         }


         // Setup value
         if (y == 0)
         {
             setValue(0.5);
         }
         else
         {
             setValue( (y / mChooserSize.height()) + 0.5);
         }
     }

     indicatorUpdate();
 }

void ColorWheel::indicatorUpdate()
{
    if(getColor().saturationF() == 0.5)
    {
        mIndicatorPosition.setX(0);
    }
    else
    {
        mIndicatorPosition.setX(mColor.saturationF() * mChooserSize.width() - (mChooserSize.width() / 2));
    }

    if(getColor().valueF() == 0.5)
    {
        mIndicatorPosition.setY(0);
    }
    else
    {
        mIndicatorPosition.setY(-(mColor.valueF() * mChooserSize.height() - (mChooserSize.height() / 2)));
    }
}

void ColorWheel::hue()
{
    if(mWheelHit)
    {
        setHue((calcAngle(mMouseVec.normalized(), QVector2D(1,0).normalized())) / 360);
    }
}

void ColorWheel::drawWheel(QPainter *inPainter)
{
    QPainterPath path;

    // Draw Whell
    inPainter->setRenderHints(QPainter::Antialiasing, true);
    inPainter->setPen(Qt::NoPen);
    inPainter->setBrush(mGradient);
    path.addEllipse(QPointF(0,0), mInnerRadius, mInnerRadius);
    path.addEllipse(QPointF(0,0), mOuterRadius, mOuterRadius);
    inPainter->drawPath(path);
}

void ColorWheel::drawChooser(QPainter *inPainter)
{
    // Draw Chooser
    QColor saturatedColor;
    saturatedColor.setHsvF(getColor().hueF(), 0, 1.0, 1.0);

    QColor valueColor(Qt::black);
    valueColor.setHsvF(getColor().hueF(), 1.0,1.0,1.0);
    saturationGradient.setColorAt(1.0, valueColor);
    saturationGradient.setColorAt(0.0, saturatedColor);

    inPainter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    inPainter->setBrush(saturationGradient);
    inPainter->drawRect(mChooserSize);
    inPainter->setBrush(valueGradient);
    inPainter->drawRect(mChooserSize );
}

void ColorWheel::drawIndicators(QPainter *inPainter)
{
    // Draw Indicators
    inPainter->setRenderHints(QPainter::Antialiasing, true);
    inPainter->setPen(QPen(Qt::black,1, Qt::SolidLine));

    // Rotate Indicator
    inPainter->save();
    inPainter->rotate(-getColor().hueF() * 360);
    inPainter->drawLine(QPointF(mInnerRadius,0), QPointF(mOuterRadius, 0));

    // Draw wheel indicator arrow
    inPainter->setPen(Qt::NoPen);
    inPainter->setBrush(Qt::black);
    inPainter->translate(mInnerRadius, 0);
    inPainter->drawConvexPolygon(arrow, 3);
    inPainter->setClipping(false);
    inPainter->restore();

    // Draw chooser indicator


    inPainter->setBrush(QColor(255,255,255,180));
    inPainter->setPen(QPen(Qt::black, indicatorBorder, Qt::SolidLine));
    inPainter->drawEllipse(mIndicatorPosition, indicatorSize, indicatorSize);
//    painter.drawRect(QRect(QPoint(mIndicatorPosition.x() - 5,
//                                  mIndicatorPosition.y() - 5),QSize(10,10)));
//    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine));
//    painter.drawPoint(mIndicatorPosition);
}


void ColorWheel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    // Move the world to the center of the screen
    painter.translate(mWorldCenter);
    drawWheel(&painter);
    drawChooser(&painter);
    drawIndicators(&painter);

    //     Temporario
    painter.end();
    painter.begin(this);
    painter.setBrush(getColor());
    painter.drawRect(0, 0, 60, 60);

}

void ColorWheel::mousePressEvent(QMouseEvent *event)
{
    mMouseVec.setX(event->x() - width() / 2);
    mMouseVec.setY(event->y() - height() / 2);

    mWheelHit = isWheelHit();
    mQuadHit = isQuadHit();

    hue();
    chooserUpdate();
    repaint();
}

void ColorWheel::mouseMoveEvent(QMouseEvent *event)
{
    mMouseVec.setX(event->x() - width() / 2);
    mMouseVec.setY(event->y() - height() / 2);

    hue();
    chooserUpdate();
    repaint();
}

void ColorWheel::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    // Move world origin to center of screen
    mWorldCenter.setX(width() / 2);
    mWorldCenter.setY(height() / 2);

    int side = std::min(width(), height()) / 2;
    mOuterRadius = side - 1;
    mInnerRadius = side - (side * 0.25);

    // Make the arrow points relative to widget size
   arrow [0] = QPoint(0, 3 * (side * 0.01));
   arrow [1] = QPoint(6 * (side * 0.01), 0);
   arrow [2] = QPoint(0, -3 * (side * 0.01));

   // Calculate the size for sampler color
   float diagonal = std::cos(physis::math::degreeToRadians(45.0)) * mInnerRadius;
   float gap = (diagonal * 0.05);

   // Calculate chooser indicator border size
   (std::min(width(), height()) < 500) ? indicatorBorder = 1 : indicatorBorder = 2;

   // Calculate chooser indicator size
   indicatorSize = mInnerRadius * 0.05;
   (indicatorSize < 1) ? indicatorSize = 1 : indicatorSize;

   // Setup the rect size for color sampler
   mChooserSize.setTopLeft(QPoint( -diagonal + gap, -diagonal + gap ));
   mChooserSize.setBottomRight(QPoint( diagonal - gap, diagonal - gap));

   saturationGradient.setStart(QPointF(-diagonal + gap, 0 ));
   saturationGradient.setFinalStop(QPointF(diagonal - gap, 0));

   valueGradient.setStart(QPointF(0, -diagonal + gap ));
   valueGradient.setFinalStop(QPointF(0, diagonal - gap));

   indicatorUpdate();

}
