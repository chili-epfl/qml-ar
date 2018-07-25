/**
 * @file qmlarplugin.h
 * @brief This class registers QMLAR
 * as a singleton object in QML
 * and QMLARMarkers image provider
 * with markers debug info
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef CELLULOARQMLPLUGIN_H
#define CELLULOARQMLPLUGIN_H

#include <QQmlExtensionPlugin>
#include <QQmlEngine>
#include <QJSEngine>

/**
 * @brief This class registers QMLAR
 * as a singleton object in QML
 * and QMLARMarkers image provider
 * with markers debug info
 */

class QMLARQMLPlugin : public QQmlExtensionPlugin
{ Q_OBJECT
  Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    /**
    * @brief Register QMLAR singleton
    */
    void registerTypes(const char *uri);

    /**
    * @brief Define the singleton type provider function (callback).
    */
    static QObject* get_qml_ar(QQmlEngine *engine, QJSEngine *scriptEngine);
};

#endif // CELLULOARQMLPLUGIN_H
