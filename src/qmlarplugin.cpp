#include "qmlarplugin.h"
#include "androidfilter.h"
#include "delayfilter.h"
#include "threadedqmlar.h"

void QMLARQMLPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("AR"));
    qmlRegisterSingletonType<QMLAR>("AR", 1, 0, "AR", get_qml_ar);
    qmlRegisterType<AndroidFilter>("AR", 1, 0, "AndroidFilter");
    qmlRegisterType<DelayFilter>("AR", 1, 0, "DelayFilter");
}

QObject *QMLARQMLPlugin::get_qml_ar(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(scriptEngine)

    ThreadedQMLAR* qml_ar_threaded = new ThreadedQMLAR();
    engine->addImageProvider("ARMarkers", qml_ar_threaded->getImageProvider());
    return qml_ar_threaded;
}
