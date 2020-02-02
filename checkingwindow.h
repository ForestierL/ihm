#ifndef CHECKINGWINDOW_H
#define CHECKINGWINDOW_H

#include <QDialog>

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
    void initializeList(QString[]);
    QString relocate();

private:
    Ui::CheckingWindow *ui;

protected slots:

private slots:
    void on_pushButton_2_clicked();
};

#endif // CHECKINGWINDOW_H
