#include "markerstorage.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>

MarkerStorage::MarkerStorage()
{
    markers.clear();
}

void MarkerStorage::populateFromFile(QString filename)
{
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    populate(file);
    file.close();
}

void MarkerStorage::populate(QString data)
{
    QJsonDocument document = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject config = document.object();

    QJsonArray markers_obj = config.value("markers").toArray();
    for(QJsonArray::iterator it = markers_obj.begin(); it != markers_obj.end(); it++)
    {
        QJsonObject marker = (*it).toObject();
        QString name = marker.value("name").toString();

        int id = marker.value("id").toInt();
        double x_mm = marker.value("x_mm").toDouble();
        double y_mm = marker.value("y_mm").toDouble();
        double size_mm = marker.value("size_mm").toDouble();

        markers[id] = Marker(x_mm, y_mm);
        markers[id].setName(name);;
        markers[id].setId(id);
        markers[id].setSizeMM(size_mm);

        qDebug() << markers[id].toString();
    }
}

void MarkerStorage::populate(QIODevice &input)
{
    if(!input.isReadable())
        qFatal("Error reading marker config");

    QString data = input.readAll();
    populate(data);
}

Marker MarkerStorage::get(int marker_id)
{
    if(markers.contains(marker_id))
        return(markers.value(marker_id));
    else
        qFatal("Marker %d does not exist in storage", marker_id);

    return Marker(0, 0);
}
