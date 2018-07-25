/**
 * @file configjson.cpp
 * @brief This class implements a JSON config reader
 * It should be inherited to add JSON functionality
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "configjson.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

ConfigJSON::ConfigJSON()
{

}

ConfigJSON::ConfigJSON(ConfigJSON &that)
{
    Q_UNUSED(that)
}

void ConfigJSON::populate(QIODevice &input)
{
    if(!input.isReadable())
        qFatal("Error reading marker config");

    QString data = input.readAll();
    populate(data);
}

void ConfigJSON::populateFromFile(QString filename)
{
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    populate(file);
    file.close();
}

void ConfigJSON::populate(QString data)
{
    QJsonDocument document = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject config = document.object();
    readConfig(config);
}
