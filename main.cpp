#include "mainwindow.h"
#include"editionwindow.h"

#include <QApplication>
#include <QLabel>
#include <QSettings>
#include "themeapplier.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setStatusBar();
    w.setNavButtons();
    new themeApplier(w);
    w.show();

    // Load an application style
    QSettings s("config.ini",QSettings::IniFormat);
    QFile styleFile(":/Ressources/" + s.value("theme").toString());
    styleFile.open(QFile::ReadOnly);

    // Apply the loaded stylesheet
    QString style(styleFile.readAll());
    a.setStyleSheet(style);

    return a.exec();
}
