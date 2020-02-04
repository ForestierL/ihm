#ifndef CHECKINGWINDOW_H
#define CHECKINGWINDOW_H

#include <QDialog>
#include <QVector>

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

protected slots:

private slots:
    void relocate(const QString);
    void on_relocateAll_clicked();
};

#endif // CHECKINGWINDOW_H
