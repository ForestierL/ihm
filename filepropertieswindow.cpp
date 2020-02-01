#include "filepropertieswindow.h"
#include "ui_filepropertieswindow.h"

FilePropertiesWindow::FilePropertiesWindow(QWidget *parent) : QDialog(parent), ui(new Ui::FilePropertiesWindow)
{
    ui->setupUi(this);
    createContents();
}

void FilePropertiesWindow::createContents()
{

}

FilePropertiesWindow::~FilePropertiesWindow()
{
    delete ui;
}
