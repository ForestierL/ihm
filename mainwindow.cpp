#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>

#include <database.h>
#include <QLabel>
#include <QPushButton>
#include "qhoversensitivebutton.h"

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

    ui->lePath->setText(mainPath);

    /*****
     * Initialisation de la liste en bas à gauche de la main windows
     * Ceci est codé en dur pour le moment (pas de bdd)
     * Il n'y a pas de limite à l'expension du layout (en hauteur)
     * ça va prendre une place énorme si on ajoute trop d'élément.
     * Il va faloir réfléchir au scroll et à la position du bouton x...
     */
    for(int i=0; i<5; i++) {
        // Création du sous-layout horizontal => label + bouton
        QLayout *layoutTest = new QHBoxLayout();

        // Création du label
        QLabel *testLabel = new QLabel();
        testLabel->setText("Test " + QString::number(i));
        // Ajout du label au sous-layout
        layoutTest->addWidget(testLabel);

        // Création du bouton
        QHoverSensitiveButton *test = new QHoverSensitiveButton(this, "delete");
        test->setMaximumSize(20,20);
        test->setStyleSheet("color:red;");
        //todo : ajouter l'intération
        // Ajout du bouton au sous-layout
        layoutTest->addWidget(test);

        // Ajout du sous-layout au layout vertical de l'UI
        ui->vlAlbums->addLayout(layoutTest);
    }

    Database database("C:\\Users\\Hibou\\Documents\\db.db");
    QString filePathTest = "test.png";
    int mark = 4;
    QString noteTest = "test : note";
    QString colorTest = "test : color";
    QString feelingTest = "test : feeling";
    database.addImage(filePathTest, mark, noteTest, colorTest, feelingTest);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_elementListView_doubleClicked(const QModelIndex &index)
{
    QString dirPath = dirModel->fileInfo(index).absoluteFilePath();

    ui->elementListView->setRootIndex(fileModel->setRootPath(dirPath));
    ui->dirTreeView->setExpanded(dirModel->setRootPath(dirPath), true);
    //ui->dirTreeView->collapse(dirModel->setRootPath(dirPath));
    ui->dirTreeView->setCurrentIndex(dirModel->setRootPath(dirPath));

    QDir pathDir(dirPath);
    if (pathDir.exists()) {
        qDebug() << "DIR " + dirPath;
        ui->lePath->setText(dirPath);//condition si dossier ou si fichier
    } else {
        qDebug() << "FILE " + dirPath;
        QMessageBox msgBox;
        msgBox.setText(dirPath);
        msgBox.exec();
        //todo : ### HERE LANCER LA FENETRE 2
        /*EditionWindow w;
        w.setImage(dirPath);
        w.createContents();
        w.show();*/
    }
}

void MainWindow::on_dirTreeView_clicked(const QModelIndex &index)
{
    QString dirPath = dirModel->fileInfo(index).absoluteFilePath();
    ui->elementListView->setRootIndex(fileModel->setRootPath(dirPath));
    ui->dirTreeView->setExpanded(dirModel->setRootPath(dirPath), true);
    //ui->dirTreeView->collapse(dirModel->setRootPath(dirPath));
    ui->dirTreeView->setCurrentIndex(dirModel->setRootPath(dirPath));
    ui->lePath->setText(dirPath);
}

void MainWindow::on_lePath_returnPressed()
{
    QString dirPath = ui->lePath->text();
    QDir pathDir(dirPath);

    if (pathDir.exists()) {
        qDebug() << "DIR " + dirPath;

        ui->elementListView->setRootIndex(fileModel->setRootPath(dirPath));
        ui->dirTreeView->setExpanded(dirModel->setRootPath(dirPath), true);
        ui->dirTreeView->setCurrentIndex(dirModel->setRootPath(dirPath));
    }
}

void MainWindow::setNavButtons(){
    QFrame *navFrame = new QFrame();
    navFrame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    navFrame->setMaximumWidth(96);
    /*navFrame->setStyleSheet("background-color: rgb(255, 0, 0);"
                               "border: 0px;"
                               "margin: 0px;"
                               "padding: 0px;");*/

    QHBoxLayout *layout = new QHBoxLayout(navFrame);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QHoverSensitiveButton *previous = new QHoverSensitiveButton(navFrame, "arrow-l");
    previous->setMaximumWidth(24);
    previous->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    /*previous->setStyleSheet("background-color: rgb(255,255,0);"
                         "color: rgb(255,255,255);"
                         "border: 0px;"
                         "margin: 0px;"
                         "padding: 0px;");*/
    QHoverSensitiveButton *next = new QHoverSensitiveButton(navFrame, "arrow-r");
    next->setMaximumWidth(24);
    next->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    /*next->setStyleSheet("background-color: rgb(0,255,0);"
                         "color: rgb(255,255,255);"
                         "border: 0px;"
                         "margin: 0px;"
                         "padding: 0px;");*/
    QHoverSensitiveButton *home = new QHoverSensitiveButton(navFrame, "home");
    home->setMaximumWidth(24);
    home->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    /*home->setStyleSheet("background-color: rgb(0,0,255);"
                         "color: rgb(255,255,255);"
                         "border: 0px;"
                         "margin: 0px;"
                         "padding: 0px;");*/
    QHoverSensitiveButton *up = new QHoverSensitiveButton(navFrame, "arrow-u");
    up->setMaximumWidth(24);
    up->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    /*up->setStyleSheet("background-color: rgb(255,0,255);"
                         "color: rgb(255,255,255);"
                         "border: 0px;"
                         "margin: 0px;"
                         "padding: 0px;");*/

    layout->addWidget(previous);
    layout->addWidget(next);
    layout->addWidget(home);
    layout->addWidget(up);

    ui->lePath->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    ui->navBar->insertWidget(0, navFrame);
    //ui->navBar->addWidget(navFrame, 1);
}

void MainWindow::setStatusBar() {
    QFrame *statusFrame = new QFrame();
    statusFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    /*statusFrame->setStyleSheet("background-color: rgb(51,51,51);"
                               "border: 0px;"
                               "margin: 0px;"
                               "padding: 0px;");*/

    QHBoxLayout *layout = new QHBoxLayout(statusFrame);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QLabel *statusMessage = new QLabel("0 élement selectionné", statusFrame);
    //statusMessage->setStyleSheet("color: white");

    QFrame *frame = new QFrame(statusFrame);
    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    /*frame->setStyleSheet("background-color: rgb(51,51,51);"
                         "border: 0px;"
                         "margin: 0px;"
                         "padding: 0px;");*/

    QHoverSensitiveButton *liste = new QHoverSensitiveButton(statusFrame, "list");
    /*liste->setStyleSheet("background-color: rgb(51, 51, 51);"
                         "color: rgb(140, 140, 140);"
                         "border: 0px;"
                         "margin: 0px;"
                         "padding: 0px;");*/
    QHoverSensitiveButton *icone = new QHoverSensitiveButton(statusFrame, "icon");
    /*icone->setStyleSheet("background-color: rgb(51, 51, 51);"
                         "color: rgb(140, 140, 140);"
                         "border: 0px;"
                         "margin: 0px;"
                         "padding: 0px;");*/
    layout->addWidget(statusMessage);
    layout->addWidget(frame);
    layout->addWidget(liste);
    layout->addWidget(icone);

    ui->statusbar->addWidget(statusFrame, 1);
    //ui->statusbar->setStyleSheet("background-color: rgb(51,51,51);");
}

void MainWindow::on_home_clicked()
{

}

void MainWindow::on_previous_clicked()
{

}

void MainWindow::on_next_clicked()
{

}

void MainWindow::on_up_clicked()
{

}

void MainWindow::on_pbAddAlbum_clicked()
{

}
