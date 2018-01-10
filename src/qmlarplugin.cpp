#include "qmlarqmlplugin.h"

void QMLARQMLPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("QMLAR"));
    qmlRegisterSingletonType<QMLAR>("QMLAR", 1, 0, "QMLAR", get_qml_ar);
}

QObject *QMLARQMLPlugin::get_qml_ar(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(scriptEngine)

    QMLAR* qml_ar = new QMLAR();
    engine->addImageProvider("QMLARMarkers", qml_ar->getImageProvider());
    return qml_ar;
}
