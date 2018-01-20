#ifndef ABSTRACTCAMERABACKEND_H
#define ABSTRACTCAMERABACKEND_H

#include <QObject>
#include "threadworker.h"

/*
 * This base class is for implementing camera backends which require a separate processing thread
 */

class AbstractCameraBackend : public QObject, public ThreadWorker
{ Q_OBJECT
public:
    AbstractCameraBackend() {}
};

#endif // ABSTRACTCAMERABACKEND_H
