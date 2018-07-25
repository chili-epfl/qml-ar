/**
 * @file imageproviderasync.cpp
 * @brief This class is an interface for image providers which emit a signal
 * on new image
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "imageproviderasync.h"

ImageProviderAsync::ImageProviderAsync() : QQuickImageProvider(QQuickImageProvider::Image)
{

}
