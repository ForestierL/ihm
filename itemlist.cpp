#include "itemlist.h"
#include "mainwindow.h"

QVector<QString> selectAllImageInDir(QString dirPath, bool recursive = false){
    qDebug() << "selectAllImageInDir >> " << dirPath;
    QDir dir(dirPath);
    QVector<QString> result;
    QStringList list = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
    foreach(QString filename, list) {
        QString path = dirPath+"/"+filename;
        if(recursive)
            result.append(selectAllImageInDir(path, recursive));
    }
    QStringList images = dir.entryList(QStringList() << "*.jpg" << "*.JPG"<< "*.png" << "*.jpeg" << "*.jpg" << "*.gif" << "*.bmp" << "*.jpe" << "*.jfif" << "*.rle" << "*.tga" << "*.tif" << "*.tiff",QDir::Files);
    foreach(QString filename, images) {
       result.append(dirPath+"/"+filename);
    }
    return result;
}

QVector<QString> selectDir(QString dirPath){
    QDir dir(dirPath);
    QVector<QString> result;
    QStringList dirs = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
    foreach(QString dir, dirs)
    {
        result.append(dirPath+"/"+dir+"/");
    }
    return result;
}

ItemList::ItemList(QWidget *parent, QString folderPath) : QWidget(parent)
{
    QVector<QString> paths = selectDir(folderPath);
    paths.append(selectAllImageInDir(folderPath));

    this->parent = parent;
    selfLayout = new QGridLayout(parent);

    this->paths = paths;
    createContent(paths);
}

ItemList::ItemList(QWidget *parent, QVector<QString> imagesPaths)
{
    this->parent = parent;
    selfLayout = new QGridLayout(parent);

    this->paths = imagesPaths;
    createContent(imagesPaths);
}


void ItemList::reloadWith(QString folderPath, bool recursive, bool showFolder, bool arrows)
{
    inAlbum = arrows;

    QFileInfo fileInfo(folderPath);
    if(!fileInfo.isDir()) //securite
    {
        QStringList qsl = folderPath.split('/');
        if(qsl.size()>1)
        {
            qsl.removeLast();
            folderPath = qsl.join("/");
        }
    }


    QVector<QString> paths;
    if(showFolder || !recursive)
        paths = selectDir(folderPath);
    if(recursive)
        paths = selectAllImageInDir(folderPath, true);
    else
        paths.append(selectAllImageInDir(folderPath, false));

    reloadWith(paths, recursive, showFolder, arrows);
}

void ItemList::reloadWith(QVector<QString> paths, bool recursive, bool showFolder, bool arrows)
{
    inAlbum = arrows;
    this->paths = paths;
    recreateContent(paths, arrows);
}

void ItemList::recreateContent(QVector<QString> paths, bool arrow)
{
    foreach(ImageItem *im, imageItems) {
        im->~ImageItem();
        im = nullptr;
    }
    imageItems.clear();

    if(paths.empty())
        return;

    for(int i=0; i<paths.size(); i++)
    {
        QString item = paths.at(i);
        ImageItem *imageItem = new ImageItem(this, item, i);
        if(imageItem->getIsImage())
            imageItem->disableMover(!arrow); //si image => on peut avoir un mover ou pas
        else
            imageItem->disableMover(true); //si dossier => pas de mover
        imageItems.append(imageItem);
        selfLayout->addWidget(imageItem,i,0);
    }
    if(arrow) {
        moverParameter();
    }
}

void ItemList::createContent(QVector<QString> paths)
{
    if(paths.empty())
        return;
    selfLayout->setSpacing(0);
    for(int i=0; i<paths.size(); i++)
    {
        QString item = paths.at(i);
        ImageItem *imageItem = new ImageItem(this, item, i);
        imageItems.append(imageItem);
        selfLayout->addWidget(imageItem,i,0);
    }
    moverParameter();
}

void ItemList::moverParameter() {
    for(ImageItem *im : imageItems)
    {
        if(im->getIsImage())
        {
            im->setDisabledUp(true);
            break;
        }
    }
    if(imageItems.last()->getIsImage())
        imageItems.last()->setDisabledDown(true);
}

QVector<ImageItem *> ItemList::getImageItems() const
{
    return imageItems;
}

void ItemList::moveTo(int currentIndex, int finalIndex)
{
    imageItems.move(currentIndex, finalIndex);

    QString imageCurrent(imageItems.at(currentIndex)->getFilePath());
    QString imageFinal(imageItems.at(finalIndex)->getFilePath());
    int idCurrent = Database::getImageId(imageCurrent);
    int idFinal = Database::getImageId(imageFinal);
    QString album = qobject_cast<MainWindow*>(this->parent->parent()->parent()->parent()->parent())->getAlbumActuel();
    int idAlbum = Database::getAlbumId(album);
    qDebug() << Database::getAlbumInImageOrderByPosition(idAlbum);
    Database::updatePositionInAlbum(idCurrent, idAlbum, currentIndex);
    Database::updatePositionInAlbum(idFinal, idAlbum, finalIndex);
    qDebug() << Database::getAlbumInImageOrderByPosition(idAlbum);

    paths.clear();
    for(int i=0; i<imageItems.size(); i++)
    {
        paths.append(imageItems.at(i)->getFilePath());
        imageItems.at(i)->~ImageItem();
    }
    imageItems.clear();

    createContent(paths);
}

void ItemList::moveUp(int currentIndex)
{
    if(currentIndex == 0)
        return;
    moveTo(currentIndex, currentIndex-1);
}

void ItemList::moveDown(int currentIndex)
{
    if(currentIndex == imageItems.size()-1)
        return;
    moveTo(currentIndex, currentIndex+1);
}

