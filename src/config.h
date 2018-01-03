#ifndef CONFIG_H
#define CONFIG_H

#include "timelogger.h"

/*
 * In this file different compile options are defined
 */

// Define to enable time output at each important step
#define PROFILE_PIPELINE

// Define to force usage of QVideoProbe
#define QT_BACKEND_FORCE_VIDEOPROBE

// implementation of PROFILE_PIPELINE
#ifdef PROFILE_PIPELINE
    #define TimeLoggerProfile(...) TimeLoggerLog(__VA_ARGS__)
#else
    #define TimeLoggerProfile(...) {}
#endif

#endif // CONFIG_H
