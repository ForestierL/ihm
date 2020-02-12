#include "itemlist.h"


QVector<QString> selectAllImageInDir(QString dirPath, bool recursive = false){
    QDir dir(dirPath);
    QVector<QString> result;
    QStringList list = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
    foreach(QString filename, list) {
        QString path = dirPath+"/"+filename;
        if(recursive)
            result.append(selectAllImageInDir(path));
    }
    QStringList images = dir.entryList(QStringList() << "*.jpg" << "*.JPG"<< "*.png" << "*.jpeg" << "*.jpg" << "*.gif" << "*.bmp" << "*.jpe" << "*.jfif" << "*.rle" << "*.tga" << "*.tif" << "*.tiff",QDir::Files);
    foreach(QString filename, images) {
       result.append(dirPath+"/"+filename);
    }
    return result;
}

ItemList::ItemList(QWidget *parent, QString folderPath, bool recursive) : QWidget(parent)
{
    QVector<QString> paths = selectAllImageInDir(folderPath, recursive);

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

void ItemList::createContent(QVector<QString> paths)
{
    // sert a quoi se setSpacing a 0?
    selfLayout->setSpacing(0);
    for(int i=0; i<paths.size(); i++)
    {
        QString item = paths.at(i);
        ImageItem *imageItem = new ImageItem(this, item, i);
        imageItems.append(imageItem);
        selfLayout->addWidget(imageItem,i,0);
    }
    imageItems.at(0)->setDisabledUp(true);
    imageItems.last()->setDisabledDown(true);
}

void ItemList::moveTo(int currentIndex, int finalIndex)
{
    imageItems.move(currentIndex, finalIndex);

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


void ItemList::createContentAlbum(QVector<int> idImages)
{
    selfLayout->setSpacing(0);
    for(int i =0;i<idImages.size();i++){
        QString filepath = Database::getImageFilePath(idImages[i]);
        ImageItem *imageItem = new ImageItem(this, filepath, i);
        imageItems.append(imageItem);
        selfLayout->addWidget(imageItem,i,0);
    }
    imageItems.at(0)->setDisabledUp(true);
    imageItems.last()->setDisabledDown(true);
}
