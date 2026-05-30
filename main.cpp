#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Creating and showing main window
    MainWindow w;
    w.show();

    return a.exec();
}
