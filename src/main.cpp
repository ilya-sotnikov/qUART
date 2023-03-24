#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a{ argc, argv };

    QCoreApplication::setOrganizationName("qUART");
    QCoreApplication::setApplicationName("qUART");

    MainWindow w;
    w.show();
    return a.exec();
}
