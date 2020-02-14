#include "mainwindow.h"
#include"editionwindow.h"

#include <QApplication>
#include <QLabel>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QSettings>
#include "themeapplier.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&translator);

    MainWindow w;
    new themeApplier(w);
    w.show();

    w.checkAllPath();

    // Load an application style
    QSettings s("config.ini",QSettings::IniFormat);
    QFile styleFile(":/Ressources/" + s.value("theme").toString());
    styleFile.open(QFile::ReadOnly);

    // Apply the loaded stylesheet
    QString style(styleFile.readAll());
    a.setStyleSheet(style);

    return a.exec();
}
