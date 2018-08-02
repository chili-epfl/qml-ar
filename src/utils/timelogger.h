/**
 * @file timelogger.h
 * @brief This class allows to output a line along with
 * thread number and a timestamp
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef TIMELOGGER_H
#define TIMELOGGER_H

#include <QString>
#include "config.h"

/**
 * @def TimeLoggerLog
 * @brief This macro provides printf-like syntax which is enabled only if ENABLE_DEBUG_OUTPUT is defined.
 * The time, thread number and time in ms since last print are also displayed.
 * @see ENABLE_DEBUG_OUTPUT
 */

#ifdef ENABLE_DEBUG_OUTPUT
// macro allowing to output file/line/function
#define TimeLoggerLog(format, ...) {TimeLogger::log(__FILE__, __LINE__, __PRETTY_FUNCTION__, format, __VA_ARGS__);}
#else
#define TimeLoggerLog(...) {}
#endif

/**
 * @brief This class allows to output a line along with
 * thread number and a timestamp
 */

class TimeLogger
{
public:
    /**
    * @brief Log data
    */
    static void log(QString filename, int line_number, QString function, const char *format, ...);
private:
    /**
    * @brief Output line to console
    */
    static void print(QString str);
};

#endif // TIMELOGGER_H
