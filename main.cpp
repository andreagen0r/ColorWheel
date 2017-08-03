#include "colorwheel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ColorWheel w;
    w.setColor(QColor(100, 130, 230, 255));
    w.show();

//    ColorWheel *r = new ColorWheel(QColor(100, 140, 220, 255));
//    r->setWindowTitle("Color Wheel");
//    r->show();

    return a.exec();
}
