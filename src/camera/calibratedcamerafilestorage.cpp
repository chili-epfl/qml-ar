#include "calibratedcamerafilestorage.h"
#include <QJsonArray>

CalibratedCameraFileStorage::CalibratedCameraFileStorage(QString filename)
{
    populateFromFile(filename);
}

void CalibratedCameraFileStorage::readConfig(QJsonObject config)
{
    // input array of doubles
    QJsonArray input_matrix = config.value("camera_matrix").toArray();

    // resulting QMatrix
    QMatrix3x3 result_matrix;

    // resulting index
    int result_idx = 0;

    // loop over input matrix
    for(QJsonArray::iterator it = input_matrix.begin(); it != input_matrix.end(); it++, result_idx++)
    {
        // current entry in the matrix
        double input_entry = (*it).toDouble();

        // copying entry to resulting matrix
        result_matrix.data()[result_idx] = input_entry;
    }

    // copying data to member variable
    // transpose required since QMatrix3x3.data()
    // is in column-major order
    A = result_matrix.transposed();
}
