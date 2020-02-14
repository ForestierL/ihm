#include "filepropertieswindow.h"
#include "ui_filepropertieswindow.h"
#include <QMessageBox>
#include "database.h"
#include <string>     // std::string, std::stoi
#include <iostream>   // std::cout
#include <QDebug>

#include "database.h"
#include <QDebug>
#include <QSettings>
#include "themeapplier.h"
#include "dominantcolorcalculator.h"

FilePropertiesWindow::FilePropertiesWindow(QWidget *parent, QString itemPath) : QDialog(parent), ui(new Ui::FilePropertiesWindow)
{
    ui->setupUi(this);
    this->itemPath = itemPath;
    idImage = Database::getImageId(itemPath);

    int timeOut = 2;
    while(idImage == -1 && timeOut!=0)
    {
        QString empty("");
        QString empty2("---");
        Database::addImage(itemPath,0,empty,empty2,empty2);
        idImage = Database::getImageId(itemPath);
        timeOut--;
    }
    createContents();
}

void FilePropertiesWindow::showEvent(QShowEvent* event){
    QDialog::showEvent(event);
    new themeApplier(*this);
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

//retourne l'extention du fichier si sans argument
//sinon on doit enc*voyer un path, retourne le path complet avec l'extention si elle est manquante
QString FilePropertiesWindow::getFileExtention(QString namePath)
{
    if(namePath.isEmpty())
    {
        QStringList qStringList = itemPath.split('.');
        return "."+qStringList.last();
    }
    else
    {
        QStringList qStringList = itemPath.split('.');
        QStringList qStringListName = namePath.split('.');
        if(qStringList.last() == qStringListName.last())
        {
            return namePath;
        }
        else
        {
            if(qStringListName.length() > 1)
            {
                qStringListName.removeLast();
                namePath = qStringListName.join('.');
            }
            return namePath+"."+qStringList.last();
        }
    }
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
                                   "Colère",
                                   "Dégoût",
                                   "Joie",
                                   "Peur",
                                   "Surprise",
                                   "Tristesse"
                               });
    loadFromBDD();
    /*setEditMode(false);*/
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

        ui->editNote->setValue(score.toInt());
        ui->editColor->setText(color);
        int index = ui->editFeelings->findText(feeling);
        ui->editFeelings->setCurrentIndex(index);
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
        ui->edit->setText("Editer");

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
        ui->edit->setText("Annuler");

        ui->editColor->show();
        connect(ui->editColor, SIGNAL(clicked()),this,SLOT(calculateColor()));
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

bool FilePropertiesWindow::imageNameChecker(QString checkName)
{
    if(checkName.isNull() || checkName.isEmpty())
    {
        return false;
    }
    QString temp(getFileExtention(getFolderPathFromImagePath(itemPath) + checkName));

    QFile qFile(temp);

    if(temp == itemPath)
        return true;
    else if(qFile.exists())
        return false;

    return true;
}

bool FilePropertiesWindow::save()
{
    if(idImage == -1) return false;

    ui->color->setText(ui->editColor->text());
    ui->feelings->setText(ui->editFeelings->currentText());
    ui->name->setText(ui->editName->text());
    ui->note->setText(QString::number(ui->editNote->value()));

    if(!imageNameChecker(ui->editName->text())) {
        return false;
    }

    QString newPath = getFolderPathFromImagePath(itemPath) + ui->editName->text();
    newPath = getFileExtention(newPath);
    ui->path->setText(newPath);

    QString color = ui->editColor->text();
    QString description = ui->description->toPlainText(); //pour faciliter l'intégration du save je laisse la ligne
    QString feeling = ui->editFeelings->currentText();
    int note = ui->editNote->value();

    qDebug() << "updateImage("<<idImage<<", "<<newPath<<", "<<note<<", "<<description<<", "<<color<<", "<<feeling<<");";
    Database::updateImage(idImage, newPath, note, description, color, feeling);

    QFile qFile(itemPath);
    qFile.rename(newPath);
    itemPath = newPath;

//    Database::up
    return true;
}

void FilePropertiesWindow::on_ok_clicked()
{
    if(!editMode){
        qobject_cast<MainWindow*>(parent())->reloadImageItem();
        accept();
    }
    else
    {
        //save
        if(!save())
        {
            QMessageBox *errorMessage = new QMessageBox(QMessageBox::Warning, "Un problème de sauvegarde est survenu", "Nous ne pouvons pas sauvegarder vos modifications.");
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

void FilePropertiesWindow::calculateColor()
{
    qDebug() << "Image color test";
    QString color = DominantColorCalculator::calculate(QImage(itemPath));
    qDebug() << "color : " << color;
    ui->color->setText(color);
    ui->editColor->setText(color);
}
