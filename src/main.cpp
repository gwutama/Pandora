#include "mainwindow.h"
#include <QApplication>
#include <QSysInfo>

int main(int argc, char* argv[])
{
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
