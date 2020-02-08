#include "themeapplier.h"
#include <QSettings>
#include <QFile>
#include <QWidget>
#include "mainwindow.h"

themeApplier::themeApplier(QMainWindow &toChange)
{
    setTheme(&toChange);
}

themeApplier::themeApplier(QWidget &toChange)
{
    setTheme(&toChange);
}

themeApplier::themeApplier(QDialog &toChange)
{
    setTheme(&toChange);
}

void themeApplier::setTheme(QWidget *window){
    QSettings s("config.ini", QSettings::IniFormat);
    QFile styleFile(":/Ressources/" + s.value("theme").toString());
    styleFile.open(QFile::ReadOnly);

    QString style(styleFile.readAll());
    window->setStyleSheet(style);

    for(int i=0; i < window->children().size(); i++)
    {
        if(QWidget *wi = qobject_cast<QWidget*>(window->children().at(i))){
            wi->setStyleSheet(style);
        }
        themeApplier::getChildAndSetStyle(window->children().at(i), s.value("theme").toString());
    }
}
void themeApplier::getChildAndSetStyle(QObject *obj, QString theme){
    QFile styleFile(":/Ressources/" + theme);
    styleFile.open(QFile::ReadOnly);

    QString style(styleFile.readAll());
    for(int i=0; i < obj->children().size(); i++)
    {
        if(QWidget *wi = qobject_cast<QWidget*>(obj->children().at(i))){
            qobject_cast<QWidget*>(obj->children().at(i))->setStyleSheet(style);
            wi->setStyleSheet(style);
        }
        if(obj->children().at(i)->children().size() > 0){
            getChildAndSetStyle(obj->children().at(i), theme);
        }
    }
}
