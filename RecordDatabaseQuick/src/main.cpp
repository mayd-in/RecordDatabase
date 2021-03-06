#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "recordmanager.h"
#include "documenthandler.h"
#include "language.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QGuiApplication::setOrganizationName("QtProject");
    QGuiApplication::setApplicationName("Record Database Example");

    qmlRegisterType<DocumentHandler>("quick.recorddatabase", 1, 0, "DocumentHandler");
    qmlRegisterType<RecordManager>("quick.recorddatabase", 1, 0, "RecordManager");
    qmlRegisterAnonymousType<Record>("quick.recorddatabase", 1);

    QQmlApplicationEngine engine;
    Language language(&engine);
    engine.rootContext()->setContextProperty("Language", &language);

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
