#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Settings settings;
    MainWindow w(&settings);
    w.show();

    return a.exec();
}
