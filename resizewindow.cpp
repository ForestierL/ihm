#include "resizewindow.h"
#include "ui_resizewindow.h"

#include <QDebug>


ResizeWindow::ResizeWindow(QWidget *parent) : QDialog(parent), ui(new Ui::ResizeWindow)
{
    ui->setupUi(this);
    setFixedSize(this->size());
    ui->lineEdit->setValidator(new QIntValidator(10, 1000, this));
}

ResizeWindow::~ResizeWindow()
{
    delete ui;
}
