#include "colorwheel.h"
#include "form.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    ColorWheel w(Qt::blue);

    Form w;
    w.show();
//    w.setColor(Qt::green);

    return a.exec();
}
