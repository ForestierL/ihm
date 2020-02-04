#include "filepropertieswindow.h"
#include "ui_filepropertieswindow.h"
#include <QMessageBox>

#include "database.h"

FilePropertiesWindow::FilePropertiesWindow(QWidget *parent, QString itemPath) : QDialog(parent), ui(new Ui::FilePropertiesWindow)
{
    ui->setupUi(this);
    this->itemPath = itemPath;
    createContents();
}

QString getNameFromPath(QString path)
{
    QStringList qStringList = path.split("/");
    return qStringList.last();
}

void FilePropertiesWindow::createContents()
{
    ui->gridLayout->setAlignment(Qt::AlignTop);
    this->setFixedSize(this->size()); //fixed size
    //ui->description->setWordWrap(true);
    ui->description->setText("Le Lorem Ipsum est simplement du faux texte employé dans la composition et la mise en page avant impression. Le Lorem Ipsum est le faux texte standard de l'imprimerie depuis les années 1500, quand un imprimeur anonyme assembla ensemble des morceaux de texte pour réaliser un livre spécimen de polices de texte. Il n'a pas fait que survivre cinq siècles, mais s'est aussi adapté à la bureautique informatique, sans que son contenu n'en soit modifié. Il a été popularisé dans les années 1960 grâce à la vente de feuilles Letraset contenant des passages du Lorem Ipsum, et, plus récemment, par son inclusion dans des applications de mise en page de texte, comme Aldus PageMaker.");
    setEditMode(false);

    ui->name->setText(getNameFromPath(itemPath));
    ui->path->setText(itemPath);

    ui->editColor->setText(ui->color->text());
    ui->editFeelings->setCurrentText(ui->feelings->text());
    ui->editName->setText(ui->name->text());
    ui->editNote->setValue(ui->note->text().toInt());
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

FilePropertiesWindow::~FilePropertiesWindow()
{
    delete ui;
}

bool FilePropertiesWindow::save()
{
    ui->color->setText(ui->editColor->text());
    ui->description->toPlainText(); //pour faciliter l'intégration du save je laisse la ligne
    ui->feelings->setText(ui->editFeelings->currentText());
    ui->name->setText(ui->editName->text());
    ui->note->setText(QString::number(ui->editNote->value()));
//    Database::up
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
            //errorMessage->setStandardButtons(QMessageBox::Ok);
            errorMessage->exec();
        }
        setEditMode(false);
    }
}

void FilePropertiesWindow::on_edit_clicked()
{
    editSwitch();
}
