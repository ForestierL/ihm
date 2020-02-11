#include "mainwindow.h"
#include"editionwindow.h"

#include <QApplication>
#include <QLabel>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&translator);

    MainWindow w;
    w.setStatusBar();
    w.setNavButtons();
    w.show();

    w.checkAllPath();

    // Load an application style
    QFile styleFile(":/Ressources/white-theme.qss");
    styleFile.open(QFile::ReadOnly);

    // Apply the loaded stylesheet
    QString style(styleFile.readAll());
    a.setStyleSheet(style);

    return a.exec();
}
