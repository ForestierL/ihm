#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qhoversensitivebutton.h"
#include "albumline.h"
#include "albumbutton.h"
#include "itemlist.h"
#include "imageitem.h"
#include "themeapplier.h"
#include"createalbumwindow.h"
#include "filterform.h"

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

    this->statusBar()->setSizeGripEnabled(false);

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
    itemList = new ItemList(ui->scrollContent_ImageItem, mainPath);

    setStatusBar();
    setNavButtons();
    updateCurrentPath(mainPath);

    ui->elementListView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->elementListView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    displayAlbum();
    createActions();

//    itemList = new ItemList(ui->scrollContent_ImageItem, mainPath);

//    ItemList *itemList = new ItemList(ui->scrollContent_ImageItem, mainPath, true);

//    itemList = new ItemList(ui->scrollContent_ImageItem, mainPath);
//    itemList->reloadWith(mainPath,false, true, true);
//    ui->elementListView->hide();
//    ui->elementListView->show();

    addRecentsAlbumToMenuFichier();
}

void MainWindow::addRecentsAlbumToMenuFichier()
{
    QVector<QString> recentAlbums = Database::getAlbumsOrderByLastModification();
    for(int i = 0; i < recentAlbums.length(); ++i)
    {
        QAction *action = new QAction(ui->menu_Albums_r_cents);
        action->setText(recentAlbums.at(i));
        ui->menu_Albums_r_cents->addAction(recentAlbums.at(i));
    }
}

void MainWindow::displayAlbum(){
    QVector<QString> albums = Database::getAlbumsOrderByName();

    for (int i = 0; i<albums.size();i++)
        createNewButtonAlbum(albums[i]);
}


void MainWindow::createActions(){
    connect(ui->actionEmplacement_r_cent, SIGNAL(triggered()), this, SLOT(recent_folder()));
    connect(ui->actionNouveau_album, SIGNAL(triggered()), this, SLOT(new_album()));
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
    connect(ui->actionTh_me_sombre, SIGNAL(triggered()), this, SLOT(dark_theme()));
    connect(ui->actionTh_me_clair, SIGNAL(triggered()), this, SLOT(light_theme()));
    connect(ui->actionA_propos, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionManuel, SIGNAL(triggered()), this, SLOT(manual()));
}

void MainWindow::recent_folder(){

}


void MainWindow::new_album(){
    CreateAlbumWindow w(this);
    //w.setImagePath(path);

    w.show();
    QEventLoop eventLoop;
    eventLoop.exec();
}
void MainWindow::add_to_album(){

}
void MainWindow::close(){
    QMainWindow::close();
}
void MainWindow::edit(){

}
void MainWindow::rename(){

    Database::getInstance();
    QVector<QString> v = Database::getAlbumsOrderByLastModification();
    for(int i = 0; i < v.size(); i++){
        qDebug() << v[i];
    }

    createActions();

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
//
}
void MainWindow::big_icons(){

}
void MainWindow::list(){

}
void MainWindow::icons(){

}
void MainWindow::about(){

}
void MainWindow::manual(){
}

void MainWindow::dark_theme(){
    QSettings s("config.ini",QSettings::IniFormat);
    s.setValue("theme", "dark");
    new themeApplier(*this);
    const QWidgetList topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget *widget : topLevelWidgets) {
        if (!widget->isHidden() && widget->isWindow())
            new themeApplier(*widget);
    }
}
void MainWindow::light_theme(){
    QSettings s("config.ini",QSettings::IniFormat);
    s.setValue("theme", "light");
    new themeApplier(*this);
    const QWidgetList topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget *widget : topLevelWidgets) {
        if (!widget->isHidden() && widget->isWindow())
            new themeApplier(*widget);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete itemList;
    delete dirModel;
    delete fileModel;
    delete pathVisit;

    ui = nullptr;
    itemList = nullptr;
    dirModel = nullptr;
    fileModel = nullptr;
    pathVisit = nullptr;
}

bool MainWindow::updateCurrentPath(QString path) {
    QListView *elementListView = ui->elementListView;
    QTreeView *dirTreeView = ui->dirTreeView;
    QLineEdit *lineEditPath = ui->lePath;

    elementListView->setRootIndex(fileModel->setRootPath(path));

    dirTreeView->setExpanded(dirModel->setRootPath(path), true);
    dirTreeView->setCurrentIndex(dirModel->setRootPath(path));

    lineEditPath->setText(path);

    itemList->reloadWith(path,false, true, true);
    qDebug() << "updateCurrentPath" << itemList->getImageItems().size();
//    statusMessage->setText(QString("%1 élément(s)").arg(itemList->getImageItems().size()));
    statusMessage->setText(QString::number(itemList->getImageItems().size()) + QString(" élément(s)"));

    pathVisit->addPath(path);
    //itemList->reloadWith(path,false, true, true);

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
    previous->setToolTip("Retour arrière");
    previous->setMaximumWidth(24);
    previous->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(previous, SIGNAL(clicked()), this, SLOT(previous_clicked()));

    QHoverSensitiveButton *next = new QHoverSensitiveButton(navFrame, "arrow-r");
    next->setToolTip("Retour avant");
    next->setMaximumWidth(24);
    next->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(next, SIGNAL(clicked()), this, SLOT(next_clicked()));

    QHoverSensitiveButton *home = new QHoverSensitiveButton(navFrame, "home");
    home->setToolTip("Retour dans le dossier d'accueil");
    home->setMaximumWidth(24);
    home->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(home, SIGNAL(clicked()), this, SLOT(home_clicked()));

    QHoverSensitiveButton *up = new QHoverSensitiveButton(navFrame, "arrow-u");
    up->setToolTip("Remonter la hierarchie");
    up->setMaximumWidth(24);
    up->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(up, SIGNAL(clicked()), this, SLOT(up_clicked()));

    QHoverSensitiveButton *allImage = new QHoverSensitiveButton(navFrame, "recursive-search");
    allImage->setToolTip("Recherche récursive dans le dossier actif");
    allImage->setMaximumWidth(24);
    allImage->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect(allImage, SIGNAL(clicked()), this, SLOT(allImage_clicked()));

    layout->addWidget(previous);
    layout->addWidget(next);
    layout->addWidget(home);
    layout->addWidget(up);

    ui->lePath->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    ui->lePath->setToolTip("Chemin du dossier actif");
    ui->navBar->insertWidget(0, navFrame);

    ui->navigation->addWidget(allImage);
}

void MainWindow::setStatusBar() {
    QFrame *statusFrame = new QFrame();
    statusFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout *layout = new QHBoxLayout(statusFrame);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    statusMessage = new QLabel("0 élement selectionné", statusFrame);
    statusMessage->setToolTip("Nombre total d'élements");

    QFrame *frame = new QFrame(statusFrame);
    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHoverSensitiveButton *liste = new QHoverSensitiveButton(statusFrame, "list");
    liste->setDisabled(true);
    liste->setToolTip("Affichage en liste (WIP)");

    QHoverSensitiveButton *icone = new QHoverSensitiveButton(statusFrame, "icon");
    icone->setDisabled(true);
    icone->setToolTip("Affichage en icones (WIP)");

    layout->addWidget(statusMessage);
    layout->addWidget(frame);
    layout->addWidget(liste);
    layout->addWidget(icone);

    ui->statusbar->addWidget(statusFrame, 1);
//    statusFrame->setStyleSheet("background-color: red;");
//    statusMessage->setStyleSheet("background-color: green;");
//    frame->setStyleSheet("background-color: yellow;");
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
        myMenu.addAction("Ouvrir", this, SLOT(openDirectory()));
        myMenu.addSeparator();
        myMenu.addAction("Supprimer", this, SLOT(removeDirectory()));

        myMenu.exec(globalPos);
    }
}

void MainWindow::openEditor(const QString path)
{
    EditionWindow w(this);
    if(path == "")
        w.setImage(actualFile);
    else
        w.setImage(path);
    w.createContents();
    w.show();
    QEventLoop eventLoop;
    eventLoop.exec();
}

void MainWindow::openDirectory()
{
    updateCurrentPath(actualFile);
}

void MainWindow::addToAlbum(const QString path)
{
    qDebug() << "AddToAlbum";
}

void MainWindow::informations(const QString path)
{
    FilePropertiesWindow w(this, actualFile);
    if(path != "")
        FilePropertiesWindow w(this, path);
    w.show();
    QEventLoop eventLoop;
    eventLoop.exec();
}

void MainWindow::eraseItem(const QString path)
{
    QFile file(actualFile);
    if(path != ""){
        file.setFileName(path);
    }

    int reponse = QMessageBox::question(this, "Suppression", "Êtes-vous sûr de vouloir supprimer cette image ?\nATTENTION ! Elle ne sera pas récupérable.", QMessageBox ::Yes | QMessageBox::No);

    if (reponse == QMessageBox::Yes)
    {
        file.remove();
        if(path != "")
            itemList->reloadWith(path,false, true, true);
    }
}

bool MainWindow::removeDirectory(QString dirPath){
    if(dirPath == ""){
        int valid = QMessageBox::question(this, "Suppression", "Êtes-vous sûr de vouloir supprimer ce dossier et tout les documents qu'il contient ? \n ATTENTION ! Ces fichiers ne pourront pas être récupérés.", QMessageBox ::Yes | QMessageBox::No);
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
    QVector<QString> allPath = Database::getAllImagePath();

    for(int i=0; i <allPath.size();i++){
        QFile file(allPath[i]);
        if (!file.exists())
        {
            missingFilesPath->append(allPath[i]);
        }
    }

    if(missingFilesPath->size() != 0){
        CheckingWindow w(this);
        w.initMissingFilesPath(missingFilesPath);
        w.show();
        QEventLoop eventLoop;
        eventLoop.exec();
    }
}

void MainWindow::on_pbAddAlbum_clicked()
{
    generateCreateAlbumLine();
}

void MainWindow::createNewButtonAlbum(QString name)
{
    AlbumButton *layoutAlbum = new AlbumButton(name);
    connect(layoutAlbum, SIGNAL(validated(const QString&)), this, SLOT(delete_album(const QString&)));
    connect(layoutAlbum, SIGNAL(openAlbum(const QString&)), this, SLOT(open_album(const QString&)));
    ui->vlAlbums->addLayout(layoutAlbum);
}


void MainWindow::generateCreateAlbumLine(){
    if(!newAlbum){
        newAlbum = true;
        // Création du sous-layout horizontal => label + bouton
        AlbumLine *layout = new AlbumLine();//QHBoxLayout();

        connect(layout, SIGNAL(validated(const QString&)), this, SLOT(create_album(const QString&)));

        // Ajout du sous-layout au layout vertical de l'UI
        ui->vlAlbums->addLayout(layout);
        layout->setFocusToAlbumTitleLineEdit();
    }
}

// Il faut modifier les messages
void MainWindow::create_album(const QString& albumName){
    QString name = albumName;
    if(name == ""){
        QMessageBox::question(this, "Nom invalide", "Veuillez entrer un nom", QMessageBox ::Ok);
    }else{
        bool result = Database::createAlbum(name);
        if(!result){
            QMessageBox::question(this, "Nom invalide", "Le nom de l'album existe deja ou est invalide", QMessageBox ::Ok);
        }else {
            delete sender();
            qDeleteAll(ui->vlAlbums->children());
            displayAlbum();
            this->newAlbum = false;
        }
    }
}

void MainWindow::delete_album(const QString& albumName){
    int reponse = QMessageBox::warning(this, "Supprimer Album", "Êtes-vous sûr de supprimer l'album ?", QMessageBox ::Yes | QMessageBox::No);
    if (reponse == QMessageBox::Yes)
    {
        QString name = albumName;
        int idAlbum = Database::getAlbumId(name);
        Database::removeAlbum(idAlbum);
        delete sender();
        qDeleteAll(ui->vlAlbums->children());
        displayAlbum();
    }
}

void MainWindow::open_album(const QString& albumName){
    QString name = albumName;

    currentDisplayedAlbumId = Database::getAlbumId(name);;
    QVector<int> idImages = Database::getAlbumInImageOrderByPosition(currentDisplayedAlbumId);
    QVector<QString> allPath;
    for(int i =0;i<idImages.size();i++){
        QString filepath = Database::getImageFilePath(idImages[i]);
        allPath.append(filepath);
    }
    itemList->reloadWith(allPath, false, false, true);
    qDebug() << "openAlbum" << itemList->getImageItems().size();
//    statusMessage->setText(QString("%1 élément(s)").arg(itemList->getImageItems().size()));
    statusMessage->setText(QString::number(itemList->getImageItems().size()) + QString(" élément(s)"));

    FilterForm *filterForm = new FilterForm(this);
    QFrame *filterFrame = new QFrame();
    filterFrame->setLayout(filterForm);

    connect(filterForm, SIGNAL(runFilter(const QString &, const QString &, const QString &)), this, SLOT(getImageFromFilter(const QString &, const QString &, const QString &)));

    ui->vlList->insertWidget(0, filterFrame);
}

void MainWindow::getImageFromFilter(const QString &color, const QString &feeling, const QString &score)
{
    Database::getImageFromFilter(currentDisplayedAlbumId, color, feeling, score);

    QVector<int> idImages = Database::getAlbumInImageOrderByPosition(currentDisplayedAlbumId);
    QVector<QString> allPath;
    for(int i =0;i<idImages.size();i++){
        QString filepath = Database::getImageFilePath(idImages[i]);
        allPath.append(filepath);
    }
    itemList->reloadWith(allPath, false, false, true);
    qDebug() << "openAlbum" << itemList->getImageItems().size();
//    statusMessage->setText(QString("%1 élément(s)").arg(itemList->getImageItems().size()));
    statusMessage->setText(QString::number(itemList->getImageItems().size()) + QString(" élément(s)"));
}

void MainWindow::allImage_clicked()
{
    int reponse = QMessageBox::warning(this, "Afficheur récursif", "Êtes-vous sûr de vouloir afficher les dossiers de manière récursive ?\nCeci peut prendre beaucoup de temps.", QMessageBox ::Yes | QMessageBox::No);
    if (reponse == QMessageBox::Yes) {
        QString path = pathVisit->getCurrentPath();
        itemList->reloadWith(path,true, false, false);
        qDebug() << "allImageClicked" << itemList->getImageItems().size();
//        statusMessage->setText(QString("%1 élément(s)").arg(itemList->getImageItems().size()));
        statusMessage->setText(QString::number(itemList->getImageItems().size()) + QString(" élément(s)"));
    }
}



Ui::MainWindow* MainWindow::getUI(void)
{
    return ui;
}
















