#include "mainwindow.h"
#include <QApplication>
#include "util.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("CloudClient");
    a.setApplicationVersion("0.26");
    a.setOrganizationName("Labo-A.L");

    Util::init();

    MainWindow w;
    w.show();
    
    return a.exec();
}
