#ifndef THEMEAPPLIER_H
#define THEMEAPPLIER_H

#include <QWidget>
#include <QMainWindow>
#include <QObject>
#include "mainwindow.h"

class themeApplier
{
public:
    themeApplier(QMainWindow&);
    themeApplier(QWidget&);
    themeApplier(QDialog&);
    void setTheme(QWidget*);
    void getChildAndSetStyle(QObject*, QString);
};

#endif // THEMEAPPLIER_H
