#include "themeapplier.h"
#include <QSettings>
#include <QFile>
#include <QWidget>
#include "mainwindow.h"
#include <QDebug>
#include "qhoversensitivebutton.h"

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
    qDebug() << "changing theme to " + s.value("theme").toString();
    QFile styleFile(":/Ressources/" + s.value("theme").toString() + "-theme.qss");
    styleFile.open(QFile::ReadOnly);

    QString style(styleFile.readAll());
    window->setStyleSheet(style);

    for(int i=0; i < window->children().size(); i++)
    {
        if(QWidget *wi = qobject_cast<QWidget*>(window->children().at(i))){
            wi->setStyleSheet(style);
        }
        qDebug() << window->children().at(i)->metaObject()->className();
        themeApplier::getChildAndSetStyle(window->children().at(i), s.value("theme").toString());
    }
}

void themeApplier::getChildAndSetStyle(QObject *obj, QString theme){
    QFile styleFile(":/Ressources/" + theme + "-theme.qss");
    styleFile.open(QFile::ReadOnly);

    QString style(styleFile.readAll());
    for(int i=0; i < obj->children().size(); i++)
    {
        qDebug() << obj->children().at(i)->metaObject()->className();
        if(QHoverSensitiveButton *wi = qobject_cast<QHoverSensitiveButton*>(obj->children().at(i))){
            qobject_cast<QWidget*>(obj->children().at(i))->setStyleSheet(style);
            wi->changeIcon();
        }
        if(obj->children().at(i)->children().size() > 0){
            getChildAndSetStyle(obj->children().at(i), theme);
        }
    }
}
