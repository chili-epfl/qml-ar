#include "celluloarqmlplugin.h"

void CelluloARQMLPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("CelluloAR"));
    qmlRegisterType<CelluloAR>(uri, 1, 0, "CelluloAR");
}

void CelluloARQMLPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);
    Q_UNUSED(engine);
    qmlRegisterSingletonType<CelluloAR>("CelluloAR", 1, 0, "CelluloAR", get_cellulo_ar);
}

QObject *CelluloARQMLPlugin::get_cellulo_ar(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(scriptEngine)

    CelluloAR* cellulo_ar = new CelluloAR();
    engine->addImageProvider("CelluloARMarkers", cellulo_ar->getImageProvider());
    return cellulo_ar;
}
