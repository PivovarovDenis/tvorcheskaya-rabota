#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("IDEF0 Generator");
    a.setApplicationVersion("1.0");
    a.setOrganizationName("IDEF0Gen");

    MainWindow w;
    w.show();
    return a.exec();
}
