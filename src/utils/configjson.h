/**
 * @file configjson.h
 * @brief This class implements a JSON config reader
 * It should be inherited to add JSON functionality
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef CONFIGJSON_H
#define CONFIGJSON_H

#include <QIODevice>
#include <QString>
#include <QJsonObject>

/**
 * @brief This class implements a JSON config reader
 * It should be inherited to add JSON functionality
 *
 * Method populate(QJsonObject), if added in your class,
 * will be called after parsing the config with populate(QString)
 * or with other available methods
 */

class ConfigJSON
{
public:
    ConfigJSON();

    ConfigJSON(ConfigJSON& that);

    virtual ~ConfigJSON() {}

    // fill with data from a file/other QIODevice
    void populate(QIODevice &input);

    /**
    * @brief Fill with data from file
    */
    void populateFromFile(QString filename);

    /**
    * @brief Fill with data from the string
    */
    void populate(QString data);

    /**
    * @brief Fill with data from the string
    */
    virtual void readConfig(QJsonObject object) = 0;
};

#endif // CONFIGJSON_H
