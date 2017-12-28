#include "timelogger.h"
#include <stdio.h>
#include <QThread>
#include <QTextStream>
#include <QDateTime>

void TimeLogger::log(QString str)
{
    QString res;
    QTextStream s(&res);
    s << QDateTime::currentMSecsSinceEpoch() << " ";
    s << ((int64_t) QThread::currentThreadId()) << " ";
    s << str;
    print(res);
}

void TimeLogger::print(QString str)
{
    fprintf(stderr, str.toStdString().c_str());
}
