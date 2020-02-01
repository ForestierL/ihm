#include "checkingwindow.h"
#include "ui_checkingwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

CheckingWindow::CheckingWindow(QWidget *parent): QDialog(parent), ui(new Ui::CheckingWindow)
{
    ui->setupUi(this);
    createContents();
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

CheckingWindow::~CheckingWindow()
{
    delete ui;
}
