/**
 * @file main.cpp
 * @brief This project prints the list of detected dots
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-10-27
 */

#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include "uchiyaprinter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // stdin/stdout
    QTextStream out(stdout);
    QTextStream in(stdin);

    // printer object
    UchiyaPrinter p;

    // input filename
    QString s;

    // input image
    QImage img;

    // list of points
    QVector<QVector2D> points;
    QVector<QVector2D>::iterator it;

    // forever
    while(true)
    {
        // reading filename
        in >> s;

        // reading image
        img = QImage(s).convertToFormat(QImage::Format_Grayscale8);

        // extracting dots
        points = p.getDots(img);

        // printing the array of dots
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
