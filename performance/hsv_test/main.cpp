#include <QCoreApplication>
#include "randomimagebackend.h"
#include "huethreshold.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    RandomImageBackend b(640, 480, 10);

    HueThreshold t;
    t.setColor(100, 10);
    t.setS(0, 100);
    t.setV(0, 100);
    a.connect(&b, &RandomImageBackend::imageAvailable, &t, &HueThreshold::setInput);

    return a.exec();
}
