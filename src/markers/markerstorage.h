/**
 * @file markerstorage.h
 * @brief This class stores the set of used markers
 * loads them from a file and outputs
 * the coordinates of each of them
 * on a request
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef MARKERSTORAGE_H
#define MARKERSTORAGE_H

#include <QMap>
#include <QIODevice>
#include "marker.h"
#include "configjson.h"

/**
 * @brief This class stores the set of used markers
 * loads them from a file and outputs
 * the coordinates of each of them
 * on a request
 *
 * coordinates: top-left corner
 *
 * INPUT/OUTPUT COORDINATE SYSTEM (OpenCV/QML):
 * x right
 * y down
 * z from reader
 */

/*
 * Json config format example:
 * {
 *   "markers": [
 *     {
 *       "name": "blue one at the top",
 *       "id": 5,
 *       "x_mm": 1.4,
 *       "y_mm": 111,
 *       "size_mm": 600
 *     },
 *     {
 *       "name": "red one in the middle",
 *       "id": 8,
 *       "x_mm": 122.1,
 *       "y_mm": 100
 *       "size_mm": 150
 *     }
 *   ]
 * }
 *
 */

class MarkerStorage : public QObject, public ConfigJSON
{ Q_OBJECT
private:
    /**
    * @brief Stored markers
    */
    QMap<int, Marker> markers;
public:
    /**
    * @brief Initialize empty storage
    */
    MarkerStorage();

    MarkerStorage(const MarkerStorage& that);

    virtual ~MarkerStorage() {}

    /**
    * @brief Fill with data from object
    */
    void readConfig(QJsonObject data);

    /**
    * @brief Resets projection matrices for each marker
    */
    void undetect();

    /**
    * @brief Obtain a marker
    * Calls qFatal on missing key
    */
    Marker get(int marker_id);

    /**
    * @brief Iterators for going through the map
    */
    QMap<int, Marker>::iterator begin();
    QMap<int, Marker>::iterator end();

    /**
    * @brief Obtain a pointer to Marker
    * Returns NULL on missing key
    */
    Marker *getPointer(int marker_id);

    /**
    * @brief Return all of the 3D-2D correspondences
    */
    WorldImageCorrespondences getCorrespondences();
    MarkerStorage &operator =(const MarkerStorage &that);

    /**
    * @brief Returns true if at least one marker was detected
    */
    bool markersDetected();
};

#endif // MARKERSTORAGE_H
