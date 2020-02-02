#include "mainwindow.h"
#include"editionwindow.h"

#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setStatusBar();
    w.setNavButtons();
    w.show();

    // Load an application style
    QFile styleFile(":/Ressources/white-theme.qss");
    styleFile.open(QFile::ReadOnly);

    // Apply the loaded stylesheet
    QString style(styleFile.readAll());
    a.setStyleSheet(style);

    return a.exec();
}
