#include "mainwindow.h"
#include <QApplication>
#include <QSysInfo>

int main(int argc, char* argv[])
{
    if (QSysInfo::productType() == "macos")
    {
        putenv((char*) "PATH=/usr/local/sbin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin");
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
