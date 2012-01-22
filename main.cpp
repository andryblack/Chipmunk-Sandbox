#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{

    QCoreApplication::setOrganizationName("AndryBlack");
    QCoreApplication::setOrganizationDomain("andryblack.com");
    QCoreApplication::setApplicationName("Chipmunk-Sandbox");

    QApplication a(argc, argv);


    MainWindow w;
    w.show();
    
    return a.exec();
}
