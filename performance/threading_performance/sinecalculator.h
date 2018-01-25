#ifndef SINECALCULATOR_H
#define SINECALCULATOR_H

#include <QObject>

class SineCalculator : public QObject
{ Q_OBJECT
private:
    qint64 max_iter;
public:
    SineCalculator(qint64 max_iter = 1000000);

public slots:
    int compute();
};

#endif // SINECALCULATOR_H
