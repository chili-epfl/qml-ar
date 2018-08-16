/**
 * @file nv21videofilter.cpp
 * @brief This class manages NV21VideoFilterRunnable
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-26
 */

#include "nv21videofilter.h"
#include "nv21videofilterrunnable.h"
#include "androidfilter.h"

NV21VideoFilterRunnable* NV21VideoFilter::runnable = nullptr;

NV21VideoFilter::NV21VideoFilter(QObject *parent) : QAbstractVideoFilter(parent)
{

}

QVideoFilterRunnable *NV21VideoFilter::createFilterRunnable()
{
    runnable = new NV21VideoFilterRunnable(this);
    return runnable;
}
