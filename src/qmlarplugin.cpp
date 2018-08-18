/**
 * @file qmlarplugin.cpp
 * @brief This class registers QMLAR
 * as a singleton object in QML
 * and QMLARMarkers image provider
 * with markers debug info
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "qmlarplugin.h"
#include "androidfilter.h"
#include "delayfilter.h"
#include "threadedqmlar.h"
#include "nv21videofilter.h"

void QMLARQMLPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("ch.epfl.chili.qmlar"));
    qmlRegisterSingletonType<QMLAR>("ch.epfl.chili.qmlar", 1, 0, "AR", get_qml_ar);
    qmlRegisterType<AndroidFilter>("ch.epfl.chili.qmlar", 1, 0, "AndroidFilter");
    qmlRegisterType<DelayFilter>("ch.epfl.chili.qmlar", 1, 0, "DelayFilter");

    // exporting AndroidFilter for API < 26 and the new NV21VideoFilter for API >= 26
#if __ANDROID_API__ >= 16
    qmlRegisterType<NV21VideoFilter>("ch.epfl.chili.qmlar", 1, 0, "NV21VideoFilter");
#else
    qmlRegisterType<AndroidFilter>("ch.epfl.chili.qmlar", 1, 0, "NV21VideoFilter");
#endif
}

QObject *QMLARQMLPlugin::get_qml_ar(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(scriptEngine)

    ThreadedQMLAR* qml_ar_threaded = new ThreadedQMLAR();
    engine->addImageProvider("ARMarkers", qml_ar_threaded->getImageProvider());
    return qml_ar_threaded;
}
