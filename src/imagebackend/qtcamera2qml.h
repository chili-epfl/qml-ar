/**
 * @file qtcamera2qml.h
 * @brief This is a wrapper over a QCamera which allows using it in QML
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef QTCAMERA2QML_H
#define QTCAMERA2QML_H

#include <QObject>
#include <QCamera>

/**
 * @brief This is a wrapper over a QCamera which allows using it in QML
 */

class QtCamera2QML : public QObject
{ Q_OBJECT

    /** @brief The property returning the media object from QCamera which is possible to use by Viewfinder in QML
     */
    Q_PROPERTY(QObject *mediaObject READ mediaObject NOTIFY never)
private:
    QCamera* camera;

public:
    /**
     * @brief QtCamera2QML wrapper contructor
     * @param cam The QCamera object
     */
    QtCamera2QML(QCamera* cam);

    /**
     * @brief Return QCamera's media object for QML
     */
    QObject* mediaObject();
signals:
    /** @brief Never called */
    void never();
};

#endif // QTCAMERA2QML_H
