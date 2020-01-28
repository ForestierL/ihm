#include "mainwindow.h"
#include"editionwindow.h"

#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setStatusBar();
    w.show();

    return a.exec();
}
