#include "addtoalbumwindow.h"
#include "ui_addtoalbumwindow.h"
#include "database.h"

#include <QMessageBox>

AddToAlbumWindow::AddToAlbumWindow(QWidget *parent, int imageId) :
    QDialog(parent),
    ui(new Ui::AddToAlbumWindow)
{
    ui->setupUi(this);
    _currentImageid = imageId;

    QVector<QString> albums = Database::getAlbumsOrderByName();

    ui->albumsComboBox->addItem("----");
    for(int i = 0; i < albums.length(); ++i)
    {
        ui->albumsComboBox->addItem(albums.at(i));
    }

     connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(ok_clicked()));
}

void AddToAlbumWindow::ok_clicked()
{
    QString albumName = ui->albumsComboBox->currentText();
    if(albumName == "----"){
        QMessageBox::question(this, "Nom invalide", "Veuillez sélectionner un album.", QMessageBox ::Ok);
    }else{
        int albumId = Database::getAlbumId(albumName);
        if(albumId == -1)
        {
            QMessageBox::question(this, "Nom invalide", "Veuillez sélectionner un autre album.", QMessageBox ::Ok);
        }
        else
        {
            bool result = Database::addImageToAlbum(_currentImageid, albumId);
            if(!result)
            {
                QMessageBox::question(this, "Erreur lors de l'ajout", "Image ou album invalide..", QMessageBox ::Ok);
            }
            else
            {
                QString msg = "Album: ";
                msg.append(albumName);
                QMessageBox::information(this, "Succes", "Image ajouté avec succées", QMessageBox ::Ok);
            }
        }
    }
}

AddToAlbumWindow::~AddToAlbumWindow()
{
    delete ui;
}
