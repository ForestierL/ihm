#include "checkingwindow.h"
#include "ui_checkingwindow.h"
#include "database.h"

#include <QDebug>
#include <QMessageBox>
#include <QSignalMapper>
#include <QSpacerItem>
#include <QSignalMapper>
#include <QFileDialog>


CheckingWindow::CheckingWindow(QWidget *parent): QDialog(parent), ui(new Ui::CheckingWindow)
{
    ui->setupUi(this);
    setFixedSize(this->size()); //fixed size
}

CheckingWindow::~CheckingWindow()
{
    delete ui;
}

void CheckingWindow::initMissingFilesPath(QVector<QString>* missingFilesPath)
{
    this->missingFilesPath = missingFilesPath;
    buttonsIgnore = new QVector<QPushButton*>();
    buttonsRelocate = new QVector<QPushButton*>();
    vlMissingList = new QVBoxLayout();

//    QLayout* vlMissingList = new QVBoxLayout();
    vlMissingList->setAlignment(Qt::AlignTop);

    for(int i=0; i < this->missingFilesPath->length(); i++)
    {
        QPushButton *relocate = new QPushButton("Relocaliser");
        QPushButton *ignore = new QPushButton("Ignorer");

        buttonsRelocate->append(relocate);
        buttonsIgnore->append(ignore);

        QLayout *layoutH = new QHBoxLayout();
        QLabel *path = new QLabel(this->missingFilesPath->at(i).split("/").last());
        path->setToolTip(this->missingFilesPath->at(i));

        QSignalMapper *mapperRelocate = new QSignalMapper();
        connect(relocate, SIGNAL(clicked()), mapperRelocate, SLOT(map()));

        mapperRelocate->setMapping(relocate, i);
        connect(mapperRelocate, SIGNAL(mapped(const int &)), this, SLOT(relocate(const int &)));

        QSignalMapper *mapperIgnore = new QSignalMapper();
        connect(ignore, SIGNAL(clicked()), mapperIgnore, SLOT(map()));
        mapperIgnore->setMapping(ignore, i);
        connect(mapperIgnore, SIGNAL(mapped(const int &)), this, SLOT(ignore(const int &)));

        // Ajout du label au sous-layout
        layoutH->addWidget(path);
        layoutH->addItem(new QSpacerItem(20,5));

        QLayout *layoutHB = new QHBoxLayout();
        // Ajout du bouton au sous-layout
        layoutHB->addWidget(buttonsRelocate->at(i));
        // Ajout du bouton au sous-layout
        layoutHB->addWidget(buttonsIgnore->at(i));

        layoutH->addItem(layoutHB);

        // Ajout du sous-layout au layout vertical de l'UI
        vlMissingList->addItem(layoutH);
    }
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->scrollArea->setWidgetResizable(false);

    QWidget *temp = new QWidget();
    temp->setLayout(vlMissingList);
    ui->scrollArea->setWidget(temp);
    ui->scrollArea->setAlignment(Qt::AlignCenter);
    //ui->scrollAreaWidgetContents->setLayout(vlMissingList);
}


void CheckingWindow::relocate(const int id){
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString());
    if(fichier!="")
    {
        QString path(missingFilesPath->at(id));
        int idImage = Database::getImageId(path); //todo : warning
        //Database::updateImagePath(idImage,fichier);
        QMessageBox::information(this, "Fichier", "Vous avez sélectionné :\n" + fichier);
    }
    else
    {
        QMessageBox::information(this, "Fichier", "Vous n'avez sélectionné aucun fichier");
    }
}


void CheckingWindow::ignore(const int id, bool all){
    int reponse = QMessageBox::Yes;
    if(!all)
        reponse = QMessageBox::question(this, "Ignorer", "Êtes-vous sûr de ne pas vouloir relocaliser cette image ?\nElle sera définitivement supprimée de tout vos albums.", QMessageBox ::Yes | QMessageBox::No);

    if (reponse == QMessageBox::Yes)
    {
        QString path = missingFilesPath->at(id);
        int idImage = Database::getImageId(path);
        Database::removeImage(idImage);
        missingFilesPath->removeAt(id);
        initMissingFilesPath(missingFilesPath);
    }

}

void CheckingWindow::on_ignoreAll_clicked()
{
    int reponse = QMessageBox::question(this, "Tout ignorer", "Êtes-vous sûr de vouloir ignorer toutes ces images ?\nElle seront définitivement supprimées de tout vos albums.", QMessageBox ::Yes | QMessageBox::No);

    if (reponse == QMessageBox::Yes)
    {
        while (missingFilesPath->size() > 0) {
            ignore(0, true);
        }
    }
    QMessageBox::information(this,"Ignorer", "Toutes les images ont été ignorées", QMessageBox ::Ok);
    CheckingWindow::close();
}
