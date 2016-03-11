#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //MainWindow w(a.arguments());
    MainWindow w(argc,argv);
    w.show();

    return a.exec();
}
