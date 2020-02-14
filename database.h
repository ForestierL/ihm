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
    static QVector<QString> getAlbumsOrderByName();
    static QVector<QString> getAlbumsOrderByLastModification();

    static int getImageId(QString &filePath);
    static bool addImage(QString &filePath, int score, QString &comment, QString &color, QString &feeling);
    static bool addImageToAlbum(int imageId, int albumId);
    static bool removeImage(int imageId);
    static int getLastImagePosition(int albumId);
    bool imageExistFromPath(QString &path);

    static bool updateImage(int idImage, QString &filePath, int score, QString &comment, QString &dominantColor, QString &feeling);
    static bool updateImagePath(int idImage, QString &filePath);
    static QVector<QString> getAllImagePath(void);
    static bool updatePositionInAlbum(int imageId, int albumId, int newPosition);
    static int getImagePosition(int imageId, int albumId);
    static bool isImageInAlbum(int imageId, int albumId);
    static QVector<QString> getInfoImage(int idImage);
    static QVector<QString> getImageFromFilter(int albumId, const QString &color, const QString &feeling, const QString &score);

    static QVector<int> getAlbumInImageOrderByPosition(int albumId);

    static QString getLastErrorMessage(void);

    static QString getImageFilePath(int idImage);

    static bool deleteImageInAlbum(int albumId, int imageId);

private:
    Database(const QString& path);

    static Database *instance;
    QSqlDatabase db;
    static QString lastErrorMessage;

};

#endif // DATABASE_H
