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
    explicit ItemList(QWidget *parent = nullptr, QString folderPath = "", bool recursive = false);
    explicit ItemList(QWidget *parent = nullptr, QVector<QString> imagesPaths = {""});
    void createContent(QVector<QString> paths = {""});
    void moveTo(int currentIndex, int finalIndex); // to 0 => prend 0 et décale la suite de 1 jusqu'à l'ancien index
    void moveUp(int currentIndex);
    void moveDown(int currentIndex);
    void createContentAlbum(QVector<int> idImages);
private:
    QVector<QWidget*> moveVectorElement(QVector<QWidget*> vector, int currentIndex, int finalIndex);
private:
    QVector<QString> paths;
    QWidget *parent;
    QGridLayout *selfLayout;
    QVector<ImageItem*> imageItems;
signals:

};

#endif // ITEMLIST_H
