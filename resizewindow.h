#ifndef RESIZEWINDOW_H
#define RESIZEWINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {class ResizeWindow;}
QT_END_NAMESPACE


class ResizeWindow : public QDialog
{
    Q_OBJECT

public:
    ResizeWindow(QWidget *parent = nullptr);
    ~ResizeWindow();

private:
    Ui::ResizeWindow *ui;

protected slots:

};

#endif // REZIZEWINDOW_H
