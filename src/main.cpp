#include "mainwindow.h"

using namespace Qt::Literals::StringLiterals;

int main(int argc, char *argv[])
{
    const QApplication a{ argc, argv };

    QCoreApplication::setOrganizationName(u"qUART"_s);
    QCoreApplication::setApplicationName(u"qUART"_s);

    MainWindow w;
    w.show();
    return a.exec();
}
