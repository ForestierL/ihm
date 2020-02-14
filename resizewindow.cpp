#include "resizewindow.h"
#include "ui_resizewindow.h"
#include "editionwindow.h"

#include <QDebug>
#include <QSignalMapper>


ResizeWindow::ResizeWindow(QImage img, QWidget *parent) : QDialog(parent), ui(new Ui::ResizeWindow)
{
    imgRef = img;
    ui->setupUi(this);
    setFixedSize(this->size());

    ui->percentValue->setText("100");
    connect(ui->percentValue, SIGNAL(textEdited(QString)), this, SLOT(modifPercent()));

    ui->heightValue->setText(QString::number(img.height()));
    connect(ui->heightValue, SIGNAL(textEdited(QString)), this, SLOT(modifHeight()));

    ui->widthValue->setText(QString::number(img.width()));
    connect(ui->widthValue, SIGNAL(textEdited(QString)), this, SLOT(modifWidth()));

    mapper = new QSignalMapper();
    connect(ui->okButton, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(ui->okButton, ui->percentValue->text().toInt());
    connect(mapper, SIGNAL(mapped(const int &)), parent, SLOT(resizePhoto(const int &)));
}

void ResizeWindow::modifPercent(){
    double percent = ui->percentValue->text().toDouble()/100;
    int height = imgRef.height();
    int width = imgRef.width();

    if(height*percent > 32000 || width*percent > 32000){
        percent = 32000/ (double) qMax(height, width);
        ui->percentValue->setText(QString::number(static_cast<int>(percent*100)));
    }

    ui->heightValue->setText(QString::number(static_cast<int>(height*percent)));
    ui->widthValue->setText(QString::number(static_cast<int>(width*percent)));
    mapper->setMapping(ui->okButton, ui->percentValue->text().toInt());
}

void ResizeWindow::modifHeight(){
    int height = ui->heightValue->text().toInt();
    int width = imgRef.width();

    if(height > 32000){ height = 32000; }
    double percent = (double) height / (double) imgRef.height();

    if(width * percent > 32000){
        percent = 32000/ (double) width;
        height = (double) imgRef.height() * percent;
    }

    ui->percentValue->setText(QString::number(static_cast<int>(percent*100)));
    ui->heightValue->setText(QString::number(static_cast<int>(height)));
    ui->widthValue->setText(QString::number(static_cast<int>(width*percent)));
    mapper->setMapping(ui->okButton, ui->percentValue->text().toInt());
}

void ResizeWindow::modifWidth(){
    int width = ui->widthValue->text().toInt();
    int height = imgRef.height();

    if(width > 32000){ width = 32000; }
    double percent = (double) width / (double) imgRef.width();

    if(height * percent > 32000){
        percent = 32000/ (double) width;
        width = (double) imgRef.width() * percent;
    }

    ui->percentValue->setText(QString::number(static_cast<int>(percent*100)));
    ui->heightValue->setText(QString::number(static_cast<int>(height*percent)));
    ui->widthValue->setText(QString::number(static_cast<int>(width)));
    mapper->setMapping(ui->okButton, ui->percentValue->text().toInt());
}

ResizeWindow::~ResizeWindow()
{
    delete ui;
}

