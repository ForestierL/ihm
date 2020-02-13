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
    ResizeWindow(QImage, QWidget *parent = nullptr);
    ~ResizeWindow();

private:
    Ui::ResizeWindow *ui;
    QImage imgRef;

protected slots:
    void modifPercent();
    void modifHeight();
    void modifWidth();
    void validate();
};

#endif // REZIZEWINDOW_H
