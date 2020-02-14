#ifndef RESIZEWINDOW_H
#define RESIZEWINDOW_H

#include <QDialog>
#include <QSignalMapper>

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
    QSignalMapper *mapper;

protected slots:
    void modifPercent();
    void modifHeight();
    void modifWidth();

};

#endif // REZIZEWINDOW_H
