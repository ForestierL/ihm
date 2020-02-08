#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qhoversensitivebutton.h"
#include "themeapplier.h"

#include <QDebug>
#include <QMessageBox>

#include <QStandardPaths>

#include <database.h>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QFileDialog>
#include <QSettings>

#include <QSplitter>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
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
    // https://stackoverflow.com/questions/14158191/qt-qtreeview-and-custom-model-with-checkbox-columns => ajouter des colones custom (note, feeling)

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

    ui->elementListView->setViewMode(QListView::IconMode);
    ui->elementListView->setMovement(QListView::Static);
    /*****
     * Initialisation de la liste en bas à gauche de la main windows
     * Ceci est codé en dur pour le moment (pas de bdd)
     * Il n'y a pas de limite à l'expansion du layout (en hauteur)
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

    Database::getInstance();
    QVector<QString> v = Database::getAlbumLast();
    for(int i = 0; i < v.size(); i++){
        qDebug() << v[i];
    }

    createActions();

    //checkAllPath(); //pour test => Lucas, pense à remove
}

void MainWindow::createActions(){
    connect(ui->actionEmplacement_r_cent, SIGNAL(triggered()), this, SLOT(recent_folder()));
    connect(ui->actionAlbum_r_cent, SIGNAL(triggered()), this, SLOT(recent_album()));
    connect(ui->actionNouveau_album, SIGNAL(triggered()), this, SLOT(new_Album()));
    connect(ui->actionAjouter_l_album, SIGNAL(triggered()), this, SLOT(add_to_album()));
    connect(ui->actionFermer, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionEditer, SIGNAL(triggered()), this, SLOT(edit()));
    connect(ui->actionRenommer, SIGNAL(triggered()), this, SLOT(rename()));
    connect(ui->actionCopier, SIGNAL(triggered()), this, SLOT(copy()));
    connect(ui->actionCouper, SIGNAL(triggered()), this, SLOT(cut()));
    connect(ui->actionColler, SIGNAL(triggered()), this, SLOT(paste()));
    connect(ui->actionIcones_petites, SIGNAL(triggered()), this, SLOT(small_icons()));
    connect(ui->actionIcones_moyennes, SIGNAL(triggered()), this, SLOT(medium_icons()));
    connect(ui->actionIcones_grandes, SIGNAL(triggered()), this, SLOT(big_icons()));
    connect(ui->actionListe, SIGNAL(triggered()), this, SLOT(list()));
    connect(ui->actionIc_nes, SIGNAL(triggered()), this, SLOT(icons()));
    connect(ui->actionA_propos, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionManuel, SIGNAL(triggered()), this, SLOT(manual()));
    connect(ui->actionTh_me_sombre, SIGNAL(triggered()), this, SLOT(dark_theme()));
    connect(ui->actionTh_me_clair, SIGNAL(triggered()), this, SLOT(light_theme()));

}

void MainWindow::recent_folder(){

}
void MainWindow::recent_album(){

}
void MainWindow::new_album(){

}
void MainWindow::add_to_album(){

}
void MainWindow::close(){

}
void MainWindow::edit(){

}
void MainWindow::rename(){

}
void MainWindow::copy(){

}
void MainWindow::cut(){

}
void MainWindow::paste(){

}
void MainWindow::small_icons(){

}
void MainWindow::medium_icons(){

}
void MainWindow::big_icons(){

}
void MainWindow::list(){

}
void MainWindow::icons(){

}
void MainWindow::dark_theme(){
    QSettings s("config.ini",QSettings::IniFormat);
    s.setValue("theme", "dark-theme.qss");
    new themeApplier(*this);
}
void MainWindow::light_theme(){
    QSettings s("config.ini",QSettings::IniFormat);
    s.setValue("theme", "light-theme.qss");
    new themeApplier(*this);
}
void MainWindow::about(){

}
void MainWindow::manual(){

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
    if (pathDir.exists())
    {
        qDebug() << "DIR " + dirPath;
        updateCurrentPath(dirPath);
    }
}

void MainWindow::setNavButtons(){
    QFrame *navFrame = new QFrame();
    navFrame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    navFrame->setMaximumWidth(96);

    QHBoxLayout *layout = new QHBoxLayout(navFrame);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QHoverSensitiveButton *previous = new QHoverSensitiveButton(navFrame, "arrow-l");
    previous->setMaximumWidth(24);
    previous->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(previous, SIGNAL(clicked()), this, SLOT(previous_clicked()));

    QHoverSensitiveButton *next = new QHoverSensitiveButton(navFrame, "arrow-r");
    next->setMaximumWidth(24);
    next->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(next, SIGNAL(clicked()), this, SLOT(next_clicked()));

    QHoverSensitiveButton *home = new QHoverSensitiveButton(navFrame, "home");
    home->setMaximumWidth(24);
    home->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(home, SIGNAL(clicked()), this, SLOT(home_clicked()));

    QHoverSensitiveButton *up = new QHoverSensitiveButton(navFrame, "arrow-u");
    up->setMaximumWidth(24);
    up->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(up, SIGNAL(clicked()), this, SLOT(up_clicked()));

    layout->addWidget(previous);
    layout->addWidget(next);
    layout->addWidget(home);
    layout->addWidget(up);

    ui->lePath->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    ui->navBar->insertWidget(0, navFrame);
}

void MainWindow::setStatusBar() {
    QFrame *statusFrame = new QFrame();
    statusFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout *layout = new QHBoxLayout(statusFrame);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QLabel *statusMessage = new QLabel("0 élement selectionné", statusFrame);
    //statusMessage->setStyleSheet("color: white");

    QFrame *frame = new QFrame(statusFrame);
    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHoverSensitiveButton *liste = new QHoverSensitiveButton(statusFrame, "list");

    QHoverSensitiveButton *icone = new QHoverSensitiveButton(statusFrame, "icon");

    layout->addWidget(statusMessage);
    layout->addWidget(frame);
    layout->addWidget(liste);
    layout->addWidget(icone);

    ui->statusbar->addWidget(statusFrame, 1);
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
    EditionWindow w(this);
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
    FilePropertiesWindow w(this, actualFile);
    //w.setImagePath(path);

    w.show();
    QEventLoop eventLoop;
    eventLoop.exec();
}

void MainWindow::eraseItem()
{
    QFile file(actualFile);

    int reponse = QMessageBox::question(this, "Suppression", "Êtes-vous sûr de vouloir supprimer cette image ?", QMessageBox ::Yes | QMessageBox::No);

    if (reponse == QMessageBox::Yes)
    {
        //bool valid = file.remove(); //modif loic => var unused
        file.remove();
    }
}

bool MainWindow::removeDirectory(QString dirPath){
    if(dirPath == ""){
        int valid = QMessageBox::question(this, "Suppression", "Êtes-vous sûr de vouloir supprimer ce dossier et tout les documents qu'il contient ?", QMessageBox ::Yes | QMessageBox::No);
        if(valid == QMessageBox::No){
            return false;
        }
        dirPath = actualFile;
    }
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

void MainWindow::home_clicked()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    pathVisit->addPath(path);
    updateCurrentPath(path);
}

void MainWindow::previous_clicked()
{
    QString path = pathVisit->back();
    updateCurrentPath(path);
}

void MainWindow::next_clicked()
{
    QString path = pathVisit->forward();
    updateCurrentPath(path);
}

void MainWindow::up_clicked()
{
    QString path = ui->lePath->text();
    QStringList pathList = path.split('/');
    pathList.removeLast();
    path = "";
    for(QString temp : pathList)
    {
        path += temp + "/";
    }
    path.resize(path.size()-1);
    updateCurrentPath(path);
}

void MainWindow::checkAllPath()
{
    QVector<QString> *missingFilesPath = new QVector<QString>();
    /*
    condition : on parse la bdd, si le path n'existe pas sur le pc alors on ajoute à missingFilePath
    */
    //DEBUG
    missingFilesPath->append("C:/UnHibou.png");
    missingFilesPath->append("Hiboubountou/photoDeTheiere.jpg");
    missingFilesPath->append("Nyan/Nyan/Nyan/Nyan.png");
    missingFilesPath->append("C:/UnHibou.png");
    missingFilesPath->append("Hiboubountou/photoDeTheiere.jpg");
    missingFilesPath->append("Nyan/Nyan/Nyan/Nyan.png");
    missingFilesPath->append("C:/UnHibou.png");
    missingFilesPath->append("Hiboubountou/photoDeTheiere.jpg");
    missingFilesPath->append("Nyan/Nyan/Nyan/Nyan.png");
    missingFilesPath->append("C:/UnHibou.png");
    missingFilesPath->append("Hiboubountou/photoDeTheiere.jpg");
    missingFilesPath->append("Nyan/Nyan/Nyan/Nyan.png");
    missingFilesPath->append("C:/UnHibou.png");
    missingFilesPath->append("Hiboubountou/photoDeTheiere.jpg");
    missingFilesPath->append("Nyan/Nyan/Nyan/Nyan.png");
    missingFilesPath->append("C:/UnHibou.png");
    missingFilesPath->append("Hiboubountou/photoDeTheiere.jpg");
    missingFilesPath->append("Nyan/Nyan/Nyan/Nyan.png");

    CheckingWindow w(this);
    w.initMissingFilesPath(missingFilesPath);
    w.show();
    QEventLoop eventLoop;
    eventLoop.exec();
}

void MainWindow::on_pbAddAlbum_clicked()
{
    generateCreateAlbumLine();
}

void MainWindow::generateCreateAlbumLine(){
    if(!newAlbum){
        newAlbum = true;
        // Création du sous-layout horizontal => label + bouton
        QLayout *layoutTest = new QHBoxLayout();

        // Création du label
        QLineEdit *albumTitle = new QLineEdit();
        albumTitle->setPlaceholderText("Titre");
        // Ajout du label au sous-layout
        layoutTest->addWidget(albumTitle);

        // Création du bouton
        QHoverSensitiveButton *test = new QHoverSensitiveButton(this, "ok");
        test->setMaximumSize(20,20);
        layoutTest->addWidget(test);

        // Ajout du sous-layout au layout vertical de l'UI
        ui->vlAlbums->addLayout(layoutTest);
    }
}





















