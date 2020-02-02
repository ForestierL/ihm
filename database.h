#ifndef DATABASE_H
#define DATABASE_H

#include <QFileSystemModel>
#include <QSqlDatabase>
#include <QSqlQuery>

class Database
{
public:
    static Database *getInstance();

    static int getAlbumId(QString &name);
    static bool createAlbum(QString &name);
    static bool removeAlbum(int id);
    static bool updateNameAlbum(QString &newName,int &id);
    static bool updateLastModifDate(int &id);
    static QVector<QString> getAlbum();
    static QVector<QString> getAlbumLast();

    static int getImageId(QString &filePath);
    static int addImage(QString &albumName, QString &filePath, int score, QString &comment, QString &color, QString &feeling);
    static bool removeImage(int imageId);

    static int getImagePosition(int imageId, int albumId);
    static bool isImageInAlbum(int imageId, int albumId);

    static void checkDBBeenCreated(void);



private:
    Database(const QString& path);

    static Database *instance;
    QSqlDatabase db;
    static QString lastErrorMessage;

};

#endif // DATABASE_H
