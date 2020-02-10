#include "database.h"
#include <QDebug>
#include <QDateTime>

#include <QSqlError>


int sqlQuerySize(QSqlQuery &query);

QString Database::lastErrorMessage;
Database *Database::instance;

Database::Database(const QString& path)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (!db.open())
    {
        qDebug() << "Error: connection with database fail";
    }
    else
    {
        qDebug() << "Database: connection ok";
    }
}

Database* Database::getInstance()
{
    if(Database::instance == nullptr){
        QString db_path = QDir::currentPath();
        qDebug() <<db_path;    //current path
        db_path =  db_path + QString("/lumipic.db");
        Database::instance = new Database("C:\\ihm\\lumipic.db"); //todo: modifier
    }

    return Database::instance;
}

/***************************************** ALBUM *********************************************/
int Database::getAlbumId(QString &name)
{
    Database::getInstance();

    QSqlQuery query("SELECT idAlbum FROM Album WHERE name = '"+name+"'");

    if(query.exec()){
        query.next();
        return query.value(0).toInt();
    } else {
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(" : Erreur lors de la requête.");
        return -1;
    }
}

QVector<QString> Database::getAlbumsOrderByName()
{
    Database::getInstance();
    QVector<QString> result;

    QSqlQuery query("SELECT name FROM Album ORDER BY Upper(name)");

    if(query.exec()){
        while (query.next()) {
            result.push_back(query.value(0).toString());
        }
        return result;
    } else {
        return result;
    }
}


QVector<QString> Database::getAlbumsOrderByLastModification()
{
    Database::getInstance();
    QVector<QString> result;

    QSqlQuery query("SELECT name FROM Album ORDER BY lastModifDate ");

    if(query.exec()){
        while (query.next()) {
            result.push_back(query.value(0).toString());
        }
        return result;
    } else {
        return result;
    }
}


bool Database::createAlbum(QString &name){
    Database::getInstance();

    int albumId;
    if((albumId = getAlbumId(name)) > 0){
        Database::lastErrorMessage = "Le nom de l'album existe deja";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO Album (name, creationDate, lastModifDate)"
                  "VALUES (:name, :creationDate, :lastModifDate)");
    query.bindValue(":name", name);
    query.bindValue(":creationDate", QDateTime::currentDateTime());
    query.bindValue(":lastModifDate", QDateTime::currentDateTime());


    if(query.exec()){
        return true;
    } else {
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(": Erreur lors de la création de l'album.");
        return false;
    }
}

bool Database::removeAlbum(int id){
    Database::getInstance();

    QSqlQuery query;
    query.prepare("DELETE FROM Album WHERE idAlbum = :idAlbum");
    query.bindValue(":idAlbum", id);

    if(query.exec()){
        return true;
    } else {
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(": Erreur lors de la suppression.");
        return false;
    }
}

bool Database::updateNameAlbum(QString &newName,int &id){
    Database::getInstance();

    QSqlQuery query;
    query.prepare("UPDATE Album SET name = :name WHERE idAlbum = :idAlbum");
    query.bindValue(":name", newName);
    query.bindValue(":idAlbum", id);

    if(query.exec()){
        return true;
    } else {
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(": Erreur lors de la requête.");
        return false;
    }
}

bool Database::updateLastModifDate(int &id){
    Database::getInstance();

    QSqlQuery query;
    query.prepare("UPDATE Album SET lastModifDate = :lastModifDate WHERE idAlbum = :idAlbum");
    query.bindValue(":lastModifDate", QDateTime::currentDateTime());
    query.bindValue(":idAlbum", id);

    if(query.exec()){
        return true;
    } else {
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(": Erreur lors de la requête.");
        return false;
    }
}
/*********************************************************************************************/



/***************************************** IMAGE *********************************************/
bool Database::addImage(QString &imagePath, int score, QString &comment, QString &dominantColor, QString &feeling)
{
    Database::getInstance();

    int imageId;
    if((imageId = getImageId(imagePath)) > 0){// vérifi que l'image n'est pas déja presente dans la BDD
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(": Image déja présente en base de données.");
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO Image (filePath, score, comment, dominantColor, feeling) "
                  "VALUES (:filePath, :score, :comment, :dominantColor, :feeling)");
    query.bindValue(":filePath", imagePath);
    query.bindValue(":score", score);
    query.bindValue(":comment", comment);
    query.bindValue(":dominantColor", dominantColor);
    query.bindValue(":feeling", feeling);

    if(query.exec()){
        return true;
    } else {
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(": Erreur lors de l'insertion.");
        return false;
    }
}

bool Database::addImageToAlbum(int imageId, int albumId)
{
    Database::getInstance();

    if(isImageInAlbum(imageId, albumId)){
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(": Image déja présente dans l'album.");
        return false;
    }

    int lastImagePosition = Database::getLastImagePosition(albumId) + 1;

    QSqlQuery query;
    query.prepare("INSERT INTO linkImageAlbum (idAlbum, idImage, positionInAlbum) "
                  "VALUES (:albumId, :imageId, :position)");
    query.bindValue(":albumId", albumId);
    query.bindValue(":imageId", imageId);
    query.bindValue(":position", lastImagePosition);

    if(query.exec()){
        return true;
    } else {
        //QSqlError error = query.lastError();
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(": Erreur lors de l'insertion.");
        return false;
    }
}

int Database::getImageId(QString &filePath)
{
    Database::getInstance();

    QSqlQuery query("SELECT idImage FROM Image WHERE filePath = '" + filePath + "';");

    if(query.exec()){
        int nbResult = sqlQuerySize(query);
        if(nbResult < 1){
            return -1;
        } else if(nbResult == 1){
            query.next();
            return query.value(0).toInt();
        } else {
            Database::lastErrorMessage = __FUNCTION__;
            Database::lastErrorMessage.append(": Plusieurs images correspondent au nom de fichier.");
            return -1; //ajouté par loic
        }
    } else {
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(" : Erreur lors de la requête.");
        return -1;
    }
}

bool Database::removeImage(int imageId)
{
    Database::getInstance();

    QSqlQuery query;
    query.prepare("DELETE FROM Image WHERE idImage = :idImage");
    query.bindValue(":idImage", imageId);

    if(query.exec()){
        return true;
    } else {
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(": Erreur lors de la suppression.");
        return false;
    }
}

int Database::getLastImagePosition(int albumId)
{
    Database::getInstance();

    QSqlQuery query;
    query.prepare("SELECT MAX(positionInAlbum) FROM linkImageAlbum WHERE idAlbum = :albumId");
    query.bindValue(":albumId", albumId);

    if(query.exec()) {
        if(query.next() > 0)
            return true;
    } else {
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(" : Erreur lors de la requête.");
    }

    return false;
}

bool Database::updateImage(int idImage, QString &filePath, int score, QString &comment, QString &dominantColor, QString &feeling)
{
    if(idImage == -1)
        return false;

    Database::getInstance();

    QSqlQuery query;
    query.prepare("UPDATE Image "
                  "SET filePath =  :filePath,"
                  "score = :score,"
                  "comment = :comment,"
                  "dominantColor = :dominantColor,"
                  "feeling = :feeling "
                  "WHERE idImage = :idImage;");
    query.bindValue(":filePath", filePath);
    query.bindValue(":score", score);
    query.bindValue(":comment", comment);
    query.bindValue(":dominantColor", dominantColor);
    query.bindValue(":feeling", feeling);
    query.bindValue(":idImage", idImage);

    if(query.exec())
    {
        if(query.next() > 0)
            return true;
        else
            return false;
    }
    else
    {
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(" : Erreur lors de la requête.");
        return false;
    }
}

bool Database::updateImagePath(int idImage, QString &filePath)
{
    if(idImage == -1)
        return false;

    Database::getInstance();

    qDebug() << idImage << " : " << filePath;
    QSqlQuery query;
    query.prepare("UPDATE Image "
                  "SET filePath =  :filePath,"
                  "score = score,"
                  "comment = comment,"
                  "dominantColor = dominantColor,"
                  "feeling = feeling "
                  "WHERE idImage = :idImage;");
    query.bindValue(":filePath", filePath);
    query.bindValue(":idImage", idImage);

    if(query.exec())
    {
        return true;
    }
    else
    {
        qDebug() << query.lastError();
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(" : Erreur lors de la requête.");
        return false;
    }
}
/*********************************************************************************************/



QVector<QString> Database::getAllImagePath()
{//TODO: tester
    Database::getInstance();

    QVector<QString> result;

    QSqlQuery query("SELECT filePath FROM Image");

    if(query.exec()){
        while (query.next()) {
            result.push_back(query.value(0).toString());
        }
    }

    return result;
}


QVector<QString> Database::getInfoImage(int imageId)
{
    Database::getInstance();

    QVector<QString> result;

    QSqlQuery query;
    query.prepare("SELECT * FROM Image WHERE idImage = :idImage");
    query.bindValue(":idImage", imageId);
    if(query.exec()){
        while (query.next()) {
            result.push_back(query.value(2).toString());
            result.push_back(query.value(3).toString());
            result.push_back(query.value(4).toString());
            result.push_back(query.value(5).toString());
        }
    }else{
        qDebug() << "rip";
    }

    return result;
}

bool Database::isImageInAlbum(int imageId, int albumId)
{
    Database::getInstance();

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM linkImageAlbum WHERE idAlbum = :idAlbum AND idImage = :idImage;");
    query.bindValue(":idAlbum", albumId);
    query.bindValue(":idImage", imageId);

    if(query.exec()) {
        query.next();
        if(query.value(0).toInt() > 0){
            return true;
        }
    } else {
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(" : Erreur lors de la requête.");
    }

    return false;
}


int Database::getImagePosition(int imageId, int albumId)
{
    Database::getInstance();

    QSqlQuery query;
    query.prepare("SELECT positionInAlbum FROM linkImageAlbum WHERE idAlbum = :idAlbum AND idImage = :idImage;");
    query.bindValue(":idAlbum", albumId);
    query.bindValue("idImage", imageId);

    if(query.exec()){
        return query.value(0).toInt();
    } else {
        QString error = __FUNCTION__;
        error.append(" : Erreur lors de la requête.");
        lastErrorMessage = error;
        return -1;
    }
}

QVector<int> Database::getAlbumInImageOrderByPosition(int albumId){
    QSqlQuery query;
    QVector<int> result;
    query.prepare("SELECT idImage FROM linkImageAlbum WHERE idAlbum = :idAlbum ORDER BY positionInAlbum");
    query.bindValue(":idAlbum", albumId);

    if(query.exec()){
        while (query.next()) {
            result.push_back(query.value(0).toInt());
        }
    }

    return result;
}

QString Database::getLastErrorMessage()
{
    return Database::lastErrorMessage;
}

int sqlQuerySize(QSqlQuery &query)
{
    int initialPos = query.at();
    int pos = 0;

    if (query.last())
        pos = query.at() + 1;
    else
        pos = 0;

    query.seek(initialPos);
    return pos;
}




/*
CREATE TABLE Album (idAlbum INTEGER CONSTRAINT pk_idAlbum PRIMARY KEY AUTOINCREMENT,
                    name STRING UNIQUE NOT NULL,
                    creationDate DATE NOT NULL,
                    lastModifDate DATE NOT NULL);


CREATE TABLE Image (idImage INTEGER CONSTRAINT pk_idImage PRIMARY KEY AUTOINCREMENT,
                    filePath STRING NOT NULL,
                    score INTEGER (1),
                    comment STRING,
                    dominantColor STRING,
                    feeling STRING);


CREATE TABLE linkImageAlbum (idAlbum INTEGER NOT NULL,
                             idImage INTEGER NOT NULL,
                             positionInAlbum INTEGER NOT NULL,
                             CONSTRAINT pk_link PRIMARY KEY (idAlbum, idImage),
                             FOREIGN KEY (idAlbum) REFERENCES Album(idAlbum),
                             FOREIGN KEY (idImage) REFERENCES Album(idImage));

*/
