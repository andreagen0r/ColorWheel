#include "colorwheel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ColorWheel w;
    w.setColor(Qt::green);
    w.show();

//    ColorWheel *r = new ColorWheel(Qt::blue);
//    r->setWindowTitle("Color Wheel");
//    r->show();

    return a.exec();
}
