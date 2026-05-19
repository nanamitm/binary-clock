#include <QApplication>
#include "ClockWidget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("BinaryClock");
    app.setOrganizationName("BinaryClock");
    app.setQuitOnLastWindowClosed(true);

    ClockWidget w;
    w.show();

    return app.exec();
}
