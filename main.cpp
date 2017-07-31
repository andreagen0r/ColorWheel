#include "colorwheel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    ColorWheel w;
//    w.setColor(Qt::green);
//    w.show();
    ColorWheel *w = new ColorWheel(Qt::blue);
    w->show();

    return a.exec();
}
