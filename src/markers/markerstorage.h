#ifndef MARKERSTORAGE_H
#define MARKERSTORAGE_H

#include <QMap>
#include <QIODevice>
#include "marker.h"
#include "configjson.h"

/*
 * This class stores the set of used markers
 * loads them from a file and outputs
 * the coordinates of each of them
 * on a request
 * coordinates: top-left corner
 *
 * INPUT/OUTPUT COORDINATE SYSTEM (OpenCV/Cellulo):
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

class MarkerStorage : public ConfigJSON
{
private:
    // stored markers
    QMap<int, Marker> markers;
public:
    // initialize empty storage
    MarkerStorage();

    // fill with data from object
    void readConfig(QJsonObject data);

    // resets projection matrices for each marker
    void undetect();

    // obtain a marker
    // calls qFatal on missing key
    Marker get(int marker_id);

    // iterators for going through the map
    QMap<int, Marker>::iterator begin();
    QMap<int, Marker>::iterator end();

    // obtain a pointer to Marker
    // returns NULL on missing key
    Marker *getPointer(int marker_id);
};

#endif // MARKERSTORAGE_H
