#include "filepropertieswindow.h"
#include "ui_filepropertieswindow.h"
#include <QMessageBox>
#include "database.h"
#include <string>     // std::string, std::stoi
#include <iostream>   // std::cout
#include <QDebug>

FilePropertiesWindow::FilePropertiesWindow(QWidget *parent, QString itemPath) : QDialog(parent), ui(new Ui::FilePropertiesWindow)
{
    ui->setupUi(this);
    this->itemPath = itemPath;
    idImage = Database::getImageId(itemPath);
    if(idImage == -1)
    {
        qDebug() << "Image " << itemPath <<" doesn't exist, image creation launched !";
        //idImage = Database::createImage();
    }
    qDebug() << "Image id : " << idImage;
}

QString getNameFromPath(QString path)
{
    QStringList qStringList = path.split("/");
    return qStringList.last();
}

QString getFolderPathFromImagePath(QString path)
{
    QStringList qStringList = path.split("/");
    qStringList.removeLast();

    return qStringList.join("/")+"/";
}

void FilePropertiesWindow::createContents()
{
    setEditMode(false);

    ui->gridLayout->setAlignment(Qt::AlignTop);
    this->setFixedSize(this->size()); //fixed size

    ui->note->setNum(0);

    ui->description->setWordWrapMode(QTextOption::WordWrap);
    ui->description->setText("");

    ui->name->setText(getNameFromPath(itemPath));
    ui->editName->setText(ui->name->text());

    ui->path->setText(itemPath);

    ui->editColor->setText("---");
    ui->color->setText("---");

    ui->feelings->setText("---");
    ui->editFeelings->addItems({
                                   "---",
                                   "Joie",
                                   "Tristesse",
                                   "Peur",
                                   "Colère",
                                   "Dégoût",
                                   "Surprise"
                               });
    loadFromBDD();
}

bool FilePropertiesWindow::loadFromBDD()
{
    if(idImage != -1){
        QVector<QString> result = Database::getInfoImage(idImage);
        QString score= result[0];
        QString comment= result[1];
        QString color = result[2];
        QString feeling = result[3];

        ui->note->setNum(score.toInt());
        ui->description->setText(comment);
        ui->color->setText(color);
        ui->feelings->setText(feeling);
        return true;
    }
    else
    {
        //normalement existera toujours
        return false;
    }
}

void FilePropertiesWindow::setEditMode(bool editMode)
{
    this->editMode = editMode;
    if(!editMode)
    {
        ui->edit->setText("Edit");

        ui->editColor->hide();
        ui->editFeelings->hide();
        ui->editName->hide();
        ui->editNote->hide();

        ui->description->setReadOnly(true);
        ui->color->show();
        ui->feelings->show();
        ui->name->show();
        ui->note->show();
    }
    else
    {
        ui->edit->setText("Cancel");

        ui->editColor->show();
        ui->editFeelings->show();
        ui->editName->show();
        ui->editNote->show();

        ui->description->setReadOnly(false);
        ui->color->hide();
        ui->feelings->hide();
        ui->name->hide();
        ui->note->hide();
    }
}

void FilePropertiesWindow::editSwitch()
{
    setEditMode(!editMode);
}

//destructeur
FilePropertiesWindow::~FilePropertiesWindow()
{
    delete ui;
}

bool FilePropertiesWindow::save()
{
    if(idImage == -1) return false;

    ui->color->setText(ui->editColor->text());
    ui->feelings->setText(ui->editFeelings->currentText());
    ui->name->setText(ui->editName->text());
    ui->note->setText(QString::number(ui->editNote->value()));

    QString newPath = getFolderPathFromImagePath(itemPath) + ui->editName->text();
    ui->path->setText(newPath);

    QString color = ui->editColor->text();
    QString description = ui->description->toPlainText(); //pour faciliter l'intégration du save je laisse la ligne
    QString feeling = ui->editFeelings->currentText();
    int note = ui->editNote->value();

    qDebug() << "updateImage("<<idImage<<", "<<newPath<<", "<<note<<", "<<description<<", "<<color<<", "<<feeling<<");";
    Database::updateImage(idImage, newPath, note, description, color, feeling);

    return true;
}

void FilePropertiesWindow::on_ok_clicked()
{
    if(!editMode)
        accept();
    else
    {
        //save
        if(!save())
        {
            QMessageBox *errorMessage = new QMessageBox(QMessageBox::Warning, "Unexpected saving problem", "We can't save your modifications.");
            errorMessage->exec();
        }
        else {
            setEditMode(false);
        }
    }
}

void FilePropertiesWindow::on_edit_clicked()
{
    editSwitch();
}
