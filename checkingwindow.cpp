#include "checkingwindow.h"
#include "ui_checkingwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

#include <QSpacerItem>

CheckingWindow::CheckingWindow(QWidget *parent): QDialog(parent), ui(new Ui::CheckingWindow)
{
    ui->setupUi(this);
    setFixedSize(this->size()); //fixed size
}

CheckingWindow::~CheckingWindow()
{
    delete ui;
}

QString getNameFromPath(QString path)
{
    QStringList qStringList = path.split("/");
    return qStringList.last();
}

void CheckingWindow::initMissingFilesPath(QVector<QString>* missingFilesPath)
{
    this->missingFilesPath = missingFilesPath;
    buttonsIgnore = new QVector<QPushButton*>();
    buttonsRelocate = new QVector<QPushButton*>();

    QLayout* vlMissingList = new QVBoxLayout();
    vlMissingList->setAlignment(Qt::AlignTop);

    for(int i=0; i < this->missingFilesPath->length(); i++)
    {
        buttonsRelocate->append(new QPushButton("Relocaliser"));
        buttonsIgnore->append(new QPushButton("Ignorer"));

        QLayout *layoutH = new QHBoxLayout();

        QLabel *path = new QLabel(getNameFromPath(this->missingFilesPath->at(i)));
        path->setToolTip(this->missingFilesPath->at(i));

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
    //LUCAS tu me changes cette fonction (nom)
    //Requête de suppression de toutes les images dans la bdd
}

