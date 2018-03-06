#ifndef CONFIG_H
#define CONFIG_H

/*
 * In this file different compile options are defined
 */

// enables TimeLogger
#define ENABLE_DEBUG_OUTPUT

// output info for measuring throughput
#define PROFILE_PIPELINE_THROUGHPUT

// output info for measuring latency
#define PROFILE_PIPELINE_LATENCY

// Define to force usage of QVideoProbe on Linux
//#define QT_BACKEND_FORCE_VIDEOPROBE

// Force Qt backend on Linux
//#define QT_BACKEND_FORCE_ON_LINUX

// implementation of PROFILE_THROUGHPUT/LATENCY
#ifdef PROFILE_PIPELINE_THROUGHPUT
    #define TimeLoggerThroughput(...) TimeLoggerLog(__VA_ARGS__)
#else
    #define TimeLoggerThroughput(...) {}
#endif

#ifdef PROFILE_PIPELINE_LATENCY
    #define TimeLoggerLatency(...) TimeLoggerLog(__VA_ARGS__)
#else
    #define TimeLoggerLatency(...) {}
#endif

#endif // CONFIG_H
