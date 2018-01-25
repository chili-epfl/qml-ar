#include "sinecalculator.h"
#include <QtMath>
#include <QDateTime>

SineCalculator::SineCalculator(qint64 max_iter)
{
    this->max_iter = max_iter;
}

int SineCalculator::compute()
{
    qint64 ms1 = QDateTime::currentDateTime().currentMSecsSinceEpoch();
    double res = 0;
    for(qint64 i = 0; i < max_iter; i++)
    {
        res += qSin(i);
    }
    return QDateTime::currentDateTime().currentMSecsSinceEpoch() - ms1;
}
