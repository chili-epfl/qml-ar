#ifndef CELLULOARQMLPLUGIN_H
#define CELLULOARQMLPLUGIN_H

#include <QQmlExtensionPlugin>
#include <QQmlEngine>
#include <QJSEngine>

/*
 * This class registers QMLAR
 * as a singleton object in QML
 * and QMLARMarkers image provider
 * with markers debug info
 */

class QMLARQMLPlugin : public QQmlExtensionPlugin
{ Q_OBJECT
  Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    // register QMLAR singleton
    void registerTypes(const char *uri);

    // Define the singleton type provider function (callback).
    static QObject* get_qml_ar(QQmlEngine *engine, QJSEngine *scriptEngine);
};

#endif // CELLULOARQMLPLUGIN_H
