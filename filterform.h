#ifndef FILTERFORM_H
#define FILTERFORM_H

#include "mainwindow.h"

#include <QObject>
#include <QHBoxLayout>
#include <QComboBox>


class FilterForm : public QHBoxLayout
{
    Q_OBJECT
public:
    explicit FilterForm(MainWindow *parent);

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

protected slots:
    void emitParamFilter(void);

signals:
    void runFilter(const QString &color, const QString &feeling, const QString &score);


};

#endif // FILTERFORM_H
