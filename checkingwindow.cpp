#include "checkingwindow.h"
#include "ui_checkingwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

CheckingWindow::CheckingWindow(QWidget *parent): QDialog(parent), ui(new Ui::CheckingWindow)
{
    int nb = 0;

    QLabel *name = new QLabel("test");
    QPushButton *bYes = new QPushButton("Relocaliser");
    QPushButton *bNo = new QPushButton("Supprimer");

    for(int i = 0; i < nb; i++){
        ui->gridLayout->addWidget(name,i,0);
        ui->gridLayout->addWidget(bYes,i,1);
        ui->gridLayout->addWidget(bNo,i,2);
    }

    ui->setupUi(this);
}

void CheckingWindow::initializeList(QString list[]){

}

QString CheckingWindow::relocate(){
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString());
    if(fichier!="")
    {
        QMessageBox::information(this, "Fichier", "Vous avez sélectionné :\n" + fichier);
    }
    else
    {
        QMessageBox::information(this, "Fichier", "Vous n'avez sélectionné aucun fichier");
    }
    return fichier;
}

void CheckingWindow::on_pushButton_2_clicked()
{
    //Requête de suppression de toutes les images dans la bdd
}

CheckingWindow::~CheckingWindow()
{
    delete ui;
}
