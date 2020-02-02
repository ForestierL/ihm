#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>

#include <QStandardPaths>

#include <database.h>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>

#include <QSplitter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*******************/
    QSplitter *splitter = new QSplitter(); //CQStandardPaths::writableLocation(QStandardPaths::PicturesLocation)réation d'un splitter, permettant de redimensionner l'espace occupé par les widgets enfants
    splitter->setOrientation(Qt::Vertical); //Mettre l'orientation verticale
    ui->leftLayout->addWidget(splitter);
    QWidget *w1 = new QWidget();
    ui->lAlbums->setParent(nullptr);
    w1->setLayout(ui->lAlbums);
    QWidget *w2 = new QWidget();
    ui->lTree->setParent(nullptr);
    w2->setLayout(ui->lTree);
    splitter->addWidget(w2);
    splitter->addWidget(w1);

    ui->lAlbums->setAlignment(Qt::AlignTop);
    /*******************/

    QString mainPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    dirModel = new QFileSystemModel(this);
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

    ui->elementListView->setModel(fileModel);

    pathVisit = new PathVisit(mainPath);
    updateCurrentPath(mainPath);

    ui->elementListView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->elementListView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
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
        test->setText("x");
        test->setMaximumSize(20,20);
        test->setStyleSheet("color:red;");
        //todo : ajouter l'intération
        // Ajout du bouton au sous-layout
        layoutTest->addWidget(test);

        // Ajout du sous-layout au layout vertical de l'UI
        ui->vlAlbums->addLayout(layoutTest);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::updateCurrentPath(QString path) {
    QListView *elementListView = ui->elementListView;
    QTreeView *dirTreeView = ui->dirTreeView;
    QLineEdit *lineEditPath = ui->lePath;

    elementListView->setRootIndex(fileModel->setRootPath(path));

    dirTreeView->setExpanded(dirModel->setRootPath(path), true);
    dirTreeView->setCurrentIndex(dirModel->setRootPath(path));

    lineEditPath->setText(path);

    pathVisit->addPath(path);

    return true;
}

void MainWindow::on_elementListView_doubleClicked(const QModelIndex &index)
{
    QString dirPath = dirModel->fileInfo(index).absoluteFilePath();
    actualFile = dirPath;

    QDir pathDir(dirPath);
    if (pathDir.exists()) {
        openDirectory();
    } else {
        openEditor();
    }
}

void MainWindow::on_dirTreeView_clicked(const QModelIndex &index)
{
    QString dirPath = dirModel->fileInfo(index).absoluteFilePath();
    updateCurrentPath(dirPath);
}

void MainWindow::on_lePath_returnPressed()
{
    QString dirPath = ui->lePath->text();

    QDir pathDir(dirPath);
    if (pathDir.exists()) {
        qDebug() << "DIR " + dirPath;
        updateCurrentPath(dirPath);
    }
}

void MainWindow::setStatusBar() {
    QFrame *statusFrame = new QFrame();
    statusFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    statusFrame->setStyleSheet("background-color: rgb(0, 0, 0);"
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
    frame->setStyleSheet("background-color: rgb(0, 0, 0);"
                         "border: 0px;"
                         "margin: 0px;"
                         "padding: 0px;");

    QPushButton *liste = new QPushButton("Liste", statusFrame);
    liste->setStyleSheet("background-color: rgb(51, 51, 51);"
                         "color: rgb(140, 140, 140);"
                         "border: 0px;"
                         "margin: 0px;"
                         "padding: 0px;");
    QPushButton *icone = new QPushButton("Icone", statusFrame);
    icone->setStyleSheet("background-color: rgb(51, 51, 51);"
                         "color: rgb(140, 140, 140);"
                         "border: 0px;"
                         "margin: 0px;"
                         "padding: 0px;");

    layout->addWidget(statusMessage);
    layout->addWidget(frame);
    layout->addWidget(liste);
    layout->addWidget(icone);

    ui->statusbar->addWidget(statusFrame, 1);
    ui->statusbar->setStyleSheet("background-color: rgb(0,0,0);");
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    // Handle global position
    QPoint globalPos = ui->elementListView->mapToGlobal(pos);
    QModelIndex itemPos = ui->elementListView->indexAt(pos);
    if(!itemPos.isValid())
    {
        return;
    }
    actualFile = ui->lePath->text() + "/" + itemPos.data().toUrl().toString();

    // Create menu and insert some actions
    QDir pathDir(actualFile);
    QMenu myMenu;

    if(!pathDir.exists())
    {
        myMenu.addAction("Ouvrir", this, SLOT(openEditor()));
        myMenu.addSeparator();
        myMenu.addAction("Ajouter à un album", this, SLOT(addToAlbum()));
        myMenu.addAction("Informations", this, SLOT(informations()));
        myMenu.addSeparator();
        myMenu.addAction("Supprimer", this, SLOT(eraseItem()));

        myMenu.exec(globalPos);
    }
    else
    {
        qDebug() << "Clique sur dossier";
        myMenu.addAction("Ouvrir", this, SLOT(openDirectory()));
        myMenu.addSeparator();
        myMenu.addAction("Supprimer", this, SLOT(removeDirectory()));

        myMenu.exec(globalPos);
    }
}

void MainWindow::openEditor()
{
    EditionWindow w;
    w.setImage(actualFile);
    w.createContents();
    w.show();
    QEventLoop eventLoop;
    eventLoop.exec();
}

void MainWindow::openDirectory()
{
    updateCurrentPath(actualFile);
    /*
    ui->elementListView->setRootIndex(fileModel->setRootPath(actualFile));
    ui->dirTreeView->setExpanded(dirModel->setRootPath(actualFile), true);
    //ui->dirTreeView->collapse(dirModel->setRootPath(dirPath));
    ui->dirTreeView->setCurrentIndex(dirModel->setRootPath(actualFile));
    ui->lePath->setText(actualFile);*/
}

void MainWindow::addToAlbum()
{
    qDebug() << "AddToAlbum";
}

void MainWindow::informations()
{
    qDebug() << "Infos";
    FilePropertiesWindow w;
    //w.setImagePath(path);
    w.createContents();
    w.show();
    QEventLoop eventLoop;
    eventLoop.exec();
}

void MainWindow::eraseItem()
{
    QFile file(actualFile);
    bool valid = file.remove();
}

//Fonction en cours de dev
bool MainWindow::removeDirectory(QString dirPath)
{
    if(dirPath == "")
        dirPath = actualFile;
    QDir folder(dirPath);
    folder.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
    foreach (QFileInfo fileInfo, folder.entryInfoList())
    {
        if(fileInfo.isDir()){
            if(!removeDirectory(fileInfo.filePath()))
                return false;
        }
        else if(fileInfo.isFile())
        {
            if(!QFile::remove(fileInfo.filePath()))
            {
                qDebug() << "Unable to remove file : " << fileInfo.filePath();
                return false;
            }
        }
    }

    if(!folder.rmdir(dirPath))
    {
        return false;
    }
    return true;
}






void MainWindow::on_pushButton_2_clicked()
{
    QString path = pathVisit->back();
    updateCurrentPath(path);
}

void MainWindow::on_pushButton_clicked()
{
    QString path = pathVisit->forward();
    updateCurrentPath(path);
}
