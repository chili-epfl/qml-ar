#include <iostream>
#include "opencvbackend.h"
using namespace std;

BackEnd::BackEnd(QObject *parent) :
    QObject(parent)
{
    setupCV();
}

int BackEnd::setupCV() {
    stream1 = new VideoCapture(0);   //0 is the id of video device.0 if you have only one camera.

    if (!stream1->isOpened()) { //check if video device has been initialised
        cout << "cannot open camera";
        exit(0);
    }

}

void BackEnd::updateImage() {
    Mat cameraFrame;
    stream1->read(cameraFrame);
    imshow("cam", cameraFrame);
}

QString BackEnd::userName()
{
    return m_userName;
}

void BackEnd::setUserName(const QString &userName)
{
    if (userName == m_userName)
        return;

    m_userName = userName;
    emit userNameChanged();
}
