#include "addtoalbumwindow.h"
#include "ui_addtoalbumwindow.h"

AddToAlbumWindow::AddToAlbumWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddToAlbumWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Ajouter à un album");
}

AddToAlbumWindow::~AddToAlbumWindow()
{
    delete ui;
}
