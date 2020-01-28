#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
<<<<<<< Updated upstream
=======

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

    ui->lePath->setText(mainPath);

    ui->home->setIcon(QIcon("E:\\Documents\\GitHub\\ihm\\home-w.png"));
    ui->up->setIcon(QIcon("E:\\Documents\\GitHub\\ihm\\arrow-u-w.png"));
    ui->previous->setIcon(QIcon("E:\\Documents\\GitHub\\ihm\\arrow-l-w.png"));
    ui->next->setIcon(QIcon("E:\\Documents\\GitHub\\ihm\\arrow-r-w.png"));

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
        QPushButton *test = new QPushButton();
        test->setMaximumSize(20,20);
        test->setStyleSheet("color:red;");
        test->setIcon(QIcon("E:\\Documents\\GitHub\\ihm\\delete-r.png"));
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

>>>>>>> Stashed changes
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setStatusBar(){
    QFrame *statusFrame = new QFrame();
    statusFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    statusFrame->setStyleSheet("background-color: rgb(255, 0, 0);"
                               "border: 0px;"
                               "margin: 0px;"
                               "padding: 0px;");

    QHBoxLayout *layout = new QHBoxLayout(statusFrame);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QLabel *statusMessage = new QLabel("0 élement selectionné", statusFrame);
    statusMessage->setStyleSheet("color: white");

    QFrame *frame = new QFrame(statusFrame);
    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    frame->setStyleSheet("background-color: rgb(255, 255, 255);"
                         "border: 0px;"
                         "margin: 0px;"
                         "padding: 0px;");

    QHoverSensitiveButton *liste = new QHoverSensitiveButton(statusFrame, "list");
    liste->setStyleSheet("background-color: rgb(51, 51, 51);"
                         "color: rgb(140, 140, 140);"
                         "border: 0px;"
                         "margin: 0px;"
                         "padding: 0px;");
    liste->setIcon(QIcon("E:\\Documents\\GitHub\\ihm\\list-w.png"));
    QHoverSensitiveButton *icone = new QHoverSensitiveButton(statusFrame, "icon");
    icone->setStyleSheet("background-color: rgb(51, 51, 51);"
                         "color: rgb(140, 140, 140);"
                         "border: 0px;"
                         "margin: 0px;"
                         "padding: 0px;");
    icone->setIcon(QIcon("E:\\Documents\\GitHub\\ihm\\icon-w.png"));
    layout->addWidget(statusMessage);
    layout->addWidget(frame);
    layout->addWidget(liste);
    layout->addWidget(icone);

    ui->statusbar->addWidget(statusFrame, 1);
    ui->statusbar->setStyleSheet("background-color: rgb(0,255,0);");
}
