#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString mainPath = "/";
    dirModel = new QFileSystemModel(this);
    dirModel->setRootPath(mainPath);
    dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    ui->dirTreeView->setModel(dirModel);
//    ui->treeView->setStyleSheet("color:white;");
    ui->dirTreeView->setHeaderHidden(true);
    for (int i = 1; i < dirModel->columnCount(); ++i)
        ui->dirTreeView->hideColumn(i);


    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs);
    QStringList sDriveFilters;
    sDriveFilters << "*.png" << "*.jpeg" << "*.jpg" << "*.gif" << "*.bmp" << "*.jpe" << "*.jfif" << "*.rle" << "*.tga" << "*.tif" << "*.tiff" ;
    fileModel->setNameFilters(sDriveFilters);
    fileModel->setNameFilterDisables(false);
    fileModel->setRootPath(mainPath);
    ui->elementListView->setModel(fileModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_elementListView_doubleClicked(const QModelIndex &index)
{
    QString dirPath = dirModel->fileInfo(index).absoluteFilePath();
    qDebug() << ">> " + dirPath;
    ui->elementListView->setRootIndex(fileModel->setRootPath(dirPath));
    ui->dirTreeView->setExpanded(dirModel->setRootPath(dirPath), true);
    //ui->dirTreeView->collapse(dirModel->setRootPath(dirPath));
    ui->dirTreeView->setCurrentIndex(dirModel->setRootPath(dirPath));
}

void MainWindow::on_dirTreeView_clicked(const QModelIndex &index)
{
    QString dirPath = dirModel->fileInfo(index).absoluteFilePath();
    ui->elementListView->setRootIndex(fileModel->setRootPath(dirPath));
    ui->dirTreeView->setExpanded(dirModel->setRootPath(dirPath), true);
    //ui->dirTreeView->collapse(dirModel->setRootPath(dirPath));
    ui->dirTreeView->setCurrentIndex(dirModel->setRootPath(dirPath));
}
