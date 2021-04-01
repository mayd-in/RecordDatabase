#include "mainwindow.h"

#include <QApplication>
#include <QScreen>
#include <QLocale>

int main(int argc, char *argv[])
{   
    QApplication app(argc, argv);

    QApplication::setOrganizationName("QtProject");
    QApplication::setApplicationName("Record Database Example");

    QLocale::setDefault(QLocale::system());

    MainWindow w;

    // Center window and give reasonable amount of size
    const QRect availableGeometry = w.screen()->availableGeometry();
    w.resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
    w.move((availableGeometry.width() - w.width()) / 2,
            (availableGeometry.height() - w.height()) / 2);

    w.show();
    return app.exec();
}
