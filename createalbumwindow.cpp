#include "createalbumwindow.h"
#include "ui_createalbumwindow.h"
#include "mainwindow.h"

CreateAlbumWindow::CreateAlbumWindow(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::CreateAlbumWindow)
{
    ui->setupUi(this);
    this->parent = parent;

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(ok_clicked()));
}

void CreateAlbumWindow::ok_clicked()
{
    QString albumName = ui->albumNameLineEdit->text();
    if(albumName == ""){
        QMessageBox::question(this, "Nom invalide", "Veuillez entrer un nom", QMessageBox ::Ok);
    }else{
        bool result = Database::createAlbum(albumName);
        if(!result){
            QMessageBox::question(this, "Nom invalide", "Le nom de l'album existe deja ou est invalide", QMessageBox ::Ok);
        }else {
            qDeleteAll(parent->getUI()->vlAlbums->children());
            parent->displayAlbum();
        }
    }
}

CreateAlbumWindow::~CreateAlbumWindow()
{
    delete ui;
}
