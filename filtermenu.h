#ifndef FILTERMENU_H
#define FILTERMENU_H

#include "mainwindow.h"
#include "qhoversensitivebutton.h"

#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>

class FilterMenu : public QHBoxLayout
{
public:
    FilterMenu(MainWindow *parent);

private:
    void initContent(void);
    void initColorComboBox(void);
    void initFeelingComboBox(void);
    void initScoreComboBox(void);
    void initDoFilterButton(void);

    MainWindow *_parent;
    QComboBox *_color;
    QComboBox *_feeling;
    QComboBox *_score;
    QHoverSensitiveButton *_doFilter;

signals:
    void runFilter(QString &color, QString &feeling, QString &score);

};

#endif // FILTERMENU_H
