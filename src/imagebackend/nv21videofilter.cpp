/**
 * @file nv21videofilter.cpp
 * @brief This class manages NV21VideoFilterRunnable
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-26
 */

#include "nv21videofilter.h"
#include "nv21videofilterrunnable.h"

NV21VideoFilterRunnable* NV21VideoFilter::runnable = nullptr;

NV21VideoFilter::NV21VideoFilter(QObject *parent) : QAbstractVideoFilter(parent)
{

}

QVideoFilterRunnable *NV21VideoFilter::createFilterRunnable()
{
#if __ANDROID_API__ >= 26
    runnable = new NV21VideoFilterRunnable(this);
    return runnable;
#else
    // this filter can be used only for Android API 26 and higher
    return nullptr;
#endif
}
