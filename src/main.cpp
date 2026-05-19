#include <QApplication>
#include <QIcon>
#include "ClockWidget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("BinaryClock");
    app.setOrganizationName("BinaryClock");
    app.setQuitOnLastWindowClosed(true);
    app.setWindowIcon(QIcon(":/resources/app.ico"));

    ClockWidget w;
    w.show();

    return app.exec();
}
