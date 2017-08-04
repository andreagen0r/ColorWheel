#include "colorwheel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ColorWheel w;
    w.setColor(QColor(100, 130, 230, 255));
    w.show();

    return a.exec();
}
