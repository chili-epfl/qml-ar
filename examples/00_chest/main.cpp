#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    // default Qt + QML init
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // loading qml
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    // starting the app
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
