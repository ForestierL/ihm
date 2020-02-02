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
    QString relocate();
    void initMissingFilesPath(QVector<QString>* missingFilesPath);

private:
    Ui::CheckingWindow *ui;
    QVector<QString> *missingFilesPath;
    QVector<QPushButton*> *buttonsRelocate;
    QVector<QPushButton*> *buttonsIgnore;

protected slots:

private slots:
    void on_pushButton_2_clicked();
};

#endif // CHECKINGWINDOW_H
