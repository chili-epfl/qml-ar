/**
 * @file timelogger.cpp
 * @brief This class allows to output a line along with
 * thread number and a timestamp
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "timelogger.h"
#include <stdio.h>
#include <QThread>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>

void TimeLogger::log(QString filename, int line_number, QString function, const char* format, ...)
{
    static qint64 last_timestamp = 0;
    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();

    // adding data to res
    QString res;
    QTextStream s(&res);

    // timestamp
    s << timestamp << "\t+" << timestamp - last_timestamp << " ms\t";

    // thread id
    s << QThread::currentThreadId() << "\t";

    // function-file-line
    s << "[" << function << "@" << filename << ":" << line_number << "]\t";

    // printf-like style implementation
    va_list argptr;
    va_start(argptr, format);

    QString str;
    str.vsprintf(format, argptr);
    s << str;

    va_end(argptr);

    // appending newline
    //s << "\n";

    // result
    print(res);

    last_timestamp = timestamp;
}

void TimeLogger::print(QString str)
{
    qDebug() << str.toStdString().c_str();
}
