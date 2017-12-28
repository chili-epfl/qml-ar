#ifndef TIMELOGGER_H
#define TIMELOGGER_H

#include <QString>

/*
 * This class allows to output a line along with
 * thread number and a timestamp
 */

class TimeLogger
{
public:
    // log data
    static void log(QString str);
private:
    // output line to console
    static void print(QString str);
};

#endif // TIMELOGGER_H
