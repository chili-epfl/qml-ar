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
    QImage img(argv[1]);

    while(true)
    {
        out << "> ";
        in >> s;
        p.getDots(QImage(s));
    }

    return a.exec();
}
