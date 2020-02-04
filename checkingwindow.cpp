#include "checkingwindow.h"
#include "ui_checkingwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QSignalMapper>

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


void CheckingWindow::initMissingFilesPath(QVector<QString>* missingFilesPath)
{
    this->missingFilesPath = missingFilesPath;
    buttonsIgnore = new QVector<QPushButton*>();
    buttonsRelocate = new QVector<QPushButton*>();

    QLayout* vlMissingList = new QVBoxLayout();
    vlMissingList->setAlignment(Qt::AlignTop);

    for(int i=0; i < this->missingFilesPath->length(); i++)
    {
        QPushButton *relocate = new QPushButton("Relocaliser");

        //connect(relocate, SIGNAL(clicked()), this, SLOT(relocate()));

        buttonsRelocate->append(relocate);
        buttonsIgnore->append(new QPushButton("Ignorer"));

        QLayout *layoutH = new QHBoxLayout();
        QLabel *path = new QLabel(this->missingFilesPath->at(i).split("/").last());
        path->setToolTip(this->missingFilesPath->at(i));

        QSignalMapper *mapper = new QSignalMapper();

        connect(relocate, SIGNAL(clicked()), mapper, SLOT(map()));
        mapper->setMapping(relocate, this->missingFilesPath->at(i));
        connect(mapper, SIGNAL(mapped(const QString &)), this, SLOT(relocate(const QString &)));

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

void CheckingWindow::relocate(const QString path){

    qDebug() << path;

    QPushButton *button = (QPushButton *)sender();

    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString());
    if(fichier!="")
    {
        QMessageBox::information(this, "Fichier", "Vous avez sélectionné :\n" + fichier);
    }
    else
    {
        QMessageBox::information(this, "Fichier", "Vous n'avez sélectionné aucun fichier");
    }
}



void CheckingWindow::on_relocateAll_clicked()
{

}
