#include "checkingwindow.h"
#include "ui_checkingwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

#include <database.h>
#include <QSpacerItem>
#include <QSignalMapper>

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
        mapperRelocate->setMapping(relocate, this->missingFilesPath->at(i));
        connect(mapperRelocate, SIGNAL(mapped(const QString &)), this, SLOT(relocate(const QString &)));

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

void CheckingWindow::relocate(QString path){
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString());
    if(fichier!="")
    {
        int idImage = Database::getImageId(path);
        Database::updateImagePath(idImage,fichier);

        QMessageBox::information(this, "Fichier", "Vous avez sélectionné :\n" + fichier);
    }
    else
    {
        QMessageBox::information(this, "Fichier", "Vous n'avez sélectionné aucun fichier");
    }
}

void CheckingWindow::ignore(const int id){
    QString path = missingFilesPath->at(id);
    int idImage = Database::getImageId(path);
    Database::removeImage(idImage);
    missingFilesPath->removeAt(id);
    initMissingFilesPath(missingFilesPath);
}

void CheckingWindow:: on_relocateAll_clicked(){

}

