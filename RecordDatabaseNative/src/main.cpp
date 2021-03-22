#include "mainwindow.h"

#include <QApplication>
#include <QScreen>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    translator.load("tr_TR.qm");

    QApplication::setOrganizationName("QtProject");
    QApplication::setApplicationName("Record Database Example");
    QApplication::setApplicationDisplayName(QApplication::translate("app", "Record Database Editor"));

    MainWindow w;

    // Center window and give reasonable amount of size
    const QRect availableGeometry = w.screen()->availableGeometry();
    w.resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
    w.move((availableGeometry.width() - w.width()) / 2,
            (availableGeometry.height() - w.height()) / 2);

    w.show();
    return app.exec();
}
