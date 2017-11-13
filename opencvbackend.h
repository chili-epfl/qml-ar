#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)

public:
    explicit BackEnd(QObject *parent = nullptr);

    int setupCV();

    QString userName();
    void setUserName(const QString &userName);

public slots:
    void updateImage();

signals:
    void userNameChanged();

private:
    VideoCapture* stream1 = NULL;
    QString m_userName;
};

#endif // BACKEND_H
