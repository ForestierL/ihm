#ifndef CHECKINGWINDOW_H
#define CHECKINGWINDOW_H

#include <QDialog>
#include <QVector>
#include <QLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class CheckingWindow;
}
QT_END_NAMESPACE

class CheckingWindow : public QDialog
{
    Q_OBJECT

public:
    CheckingWindow(QWidget *parent = nullptr);
    ~CheckingWindow();
    void initMissingFilesPath(QVector<QString>* missingFilesPath);

private:
    Ui::CheckingWindow *ui;
    QVector<QString> *missingFilesPath;
    QVector<QPushButton*> *buttonsRelocate;
    QVector<QPushButton*> *buttonsIgnore;
    QLayout* vlMissingList;

protected slots:

private slots:
    void relocate(const int);
    void ignore(const int, bool all = false);
    void on_relocateAll_clicked();
    void on_ignoreAll_clicked();
};

#endif // CHECKINGWINDOW_H
