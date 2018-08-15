/**
 * @file config.h
 * @brief In this file different compile options are defined
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef CONFIG_H
#define CONFIG_H

/*
 *
 */

/**
* @def ENABLE_DEBUG_OUTPUT
* @brief Enables TimeLogger
*/
#define ENABLE_DEBUG_OUTPUT

/**
* @def PROFILE_PIPELINE_THROUGHPUT
* @brief Output info for measuring throughput
*/
//#define PROFILE_PIPELINE_THROUGHPUT

/**
* @def PROFILE_PIPELINE_LATENCY
* @brief Output info for measuring latency
*/
#define PROFILE_PIPELINE_LATENCY
//#undef PROFILE_PIPELINE_LATENCY

/**
* @def QT_BACKEND_FORCE_VIDEOPROBE
* @brief Define to force usage of QVideoProbe on Linux
*/
//#define QT_BACKEND_FORCE_VIDEOPROBE
#undef QT_BACKEND_FORCE_VIDEOPROBE

/**
* @def QT_BACKEND_FORCE_ON_LINUX
* @brief Force Qt backend on Linux
*/
//#define QT_BACKEND_FORCE_ON_LINUX
#undef QT_BACKEND_FORCE_ON_LINUX

/**
* @def TimeLoggerThroughput
* @brief printf-like syntax, enabled only if PROFILE_PIPELINE_THROUGHPUT is defined
* @see PROFILE_PIPELINE_THROUGHPUT
*/

// implementation of PROFILE_THROUGHPUT/LATENCY
#ifdef PROFILE_PIPELINE_THROUGHPUT
#define TimeLoggerThroughput(...) TimeLoggerLog(__VA_ARGS__)
#else
#define TimeLoggerThroughput(...) {}
#endif

/**
* @def TimeLoggerLatency
* @brief printf-like syntax, enabled only if PROFILE_PIPELINE_LATENCY is defined
* @see PROFILE_PIPELINE_LATENCY
*/

#ifdef PROFILE_PIPELINE_LATENCY
#define TimeLoggerLatency(...) TimeLoggerLog(__VA_ARGS__)
#else
#define TimeLoggerLatency(...) {}
#endif

#endif // CONFIG_H
