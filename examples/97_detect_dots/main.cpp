#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include "uchiyaprinter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextStream out(stdout);
    QTextStream in(stdin);
    UchiyaPrinter p;
    QString s;
    QImage img;
    QVector<QVector2D>::iterator it;
    QVector<QVector2D> points;

    while(true)
    {
        in >> s;
        img = QImage(s).convertToFormat(QImage::Format_Grayscale8);
        points = p.getDots(img);
        out << points.length() << "\n";
        for(it = points.begin(); it != points.end(); it++)
        {
            QVector2D pt = *it;
            out << pt.x() << " " << pt.y() << "\n";
        }
        out.flush();
    }

    return a.exec();
}
