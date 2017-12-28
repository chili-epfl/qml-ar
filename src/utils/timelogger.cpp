#include "timelogger.h"
#include <stdio.h>
#include <QThread>
#include <QTextStream>
#include <QDateTime>

void TimeLogger::log(QString filename, int line_number, QString function, const char* format, ...)
{
    // adding data to res
    QString res;
    QTextStream s(&res);

    // timestamp
    s << QDateTime::currentMSecsSinceEpoch() << " ";

    // thread id
    s << ((int64_t) QThread::currentThreadId()) << " ";

    // function-file-line
    s << "[" << function << "@" << filename << ":" << line_number << "] ";

    // printf-like style implementation
    va_list argptr;
    va_start(argptr, format);

    QString str;
    str.vsprintf(format, argptr);
    s << str;

    va_end(argptr);

    // appending newline
    s << "\n";

    // result
    print(res);
}

void TimeLogger::print(QString str)
{
    fprintf(stderr, str.toStdString().c_str());
}
