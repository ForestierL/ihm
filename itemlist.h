#ifndef ITEMLIST_H
#define ITEMLIST_H

#include <QWidget>
#include "imageitem.h"
#include <QLineEdit>
#include <QPushButton>
#include <QDir>
#include "database.h"
class ItemList : public QWidget
{
    Q_OBJECT
public:
    explicit ItemList(QWidget *parent = nullptr, QString folderPath = "");
    explicit ItemList(QWidget *parent = nullptr, QVector<QString> imagesPaths = {""});
    void reloadWith(QString folderPath, bool recursive = false, bool showFolder = true, bool arrows = false);
    void reloadWith(QVector<QString> paths, bool recursive = false, bool showFolder = true, bool arrows = false);
    void moveTo(int currentIndex, int finalIndex); // to 0 => prend 0 et décale la suite de 1 jusqu'à l'ancien index
    void moveUp(int currentIndex);
    void moveDown(int currentIndex);
    void createContentAlbum(QVector<int> idImages);
private:
    void createContent(QVector<QString> paths = {""});
    void recreateContent(QVector<QString> paths = {""}, bool arrow = false);
    QVector<QWidget*> moveVectorElement(QVector<QWidget*> vector, int currentIndex, int finalIndex);
    void moverParameter();
private:
    QVector<QString> paths;
    QWidget *parent;
    QGridLayout *selfLayout;
    QVector<ImageItem*> imageItems;
signals:

};

#endif // ITEMLIST_H
