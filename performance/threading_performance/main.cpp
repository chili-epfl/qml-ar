#include <QCoreApplication>
#include <QtConcurrent>
#include "sinecalculator.h"
#include <QVector>

QVector<int> run(int n)
{
    QVector<int> result;
    QVector<QFuture<int>> futures;
    for(int i = 0; i < n; i++)
    {
        SineCalculator calc(500000000L);
        futures.append(QtConcurrent::run(&calc, &SineCalculator::compute));
    }
    for(int i = 0; i < n; i++)
        futures[i].waitForFinished();

    for(int i = 0; i < n; i++)
        result.append(futures[i].result());

    return result;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QThreadPool::globalInstance()->setMaxThreadCount(4);

//    for(int i = 0; i < 10; i++)
//        qDebug() << run(i);

    run(4);

    return a.exec();
}
