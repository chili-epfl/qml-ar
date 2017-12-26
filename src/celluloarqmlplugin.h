#ifndef CELLULOARQMLPLUGIN_H
#define CELLULOARQMLPLUGIN_H

#include "celluloar.h"
#include <QQmlExtensionPlugin>

/*
 * This class registers CelluloAR
 * as a singleton object in QML
 * and CelluloARMarkers image provider
 * with markers debug info
 */

class CelluloARQMLPlugin : public QQmlExtensionPlugin
{ Q_OBJECT
  Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri);
    void initializeEngine(QQmlEngine *engine, const char *uri);

    // Second, define the singleton type provider function (callback).
    static QObject* get_cellulo_ar(QQmlEngine *engine, QJSEngine *scriptEngine);
};

#endif // CELLULOARQMLPLUGIN_H
