#include "database.h"
#include <QDebug>
#include <QDateTime>


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
        Database::instance = new Database("/Users/romaincolonnadistria/Desktop/ihm/lumipic.db"); //todo: modifier
    }

    return Database::instance;
}

/***************************************** ALBUM *********************************************/
int Database::getAlbumId(QString &name)
{
    Database::checkDBBeenCreated();

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

QVector<QString> Database::getAlbum()
{
    QVector<QString> result;
    Database::checkDBBeenCreated();

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


QVector<QString> Database::getAlbumLast()
{
    QVector<QString> result;
    Database::checkDBBeenCreated();

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
    Database::checkDBBeenCreated();

    int albumId;
    if((albumId = getAlbumId(name)) > 0){
        Database::lastErrorMessage = "Le nom de l'album existe deja";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO Album (name, creationDate,lastModifDate)"
                  "VALUES (:name, :creationDate,:lastModifDate)");
    query.bindValue(":name", name);
    query.bindValue(":creationDate", QDateTime::currentDateTime());
    query.bindValue(":lastModifDate", QDateTime::currentDateTime());

    if(query.exec()){
        return true;
    }else {
        return false;
    }
}

bool Database::removeAlbum(int id){
    Database::checkDBBeenCreated();
    QSqlQuery query;
    query.prepare("DELETE FROM Album WHERE idAlbum = :idAlbum");
    query.bindValue(":idAlbum", id);
    if(query.exec()){
        return true;
    }else {
        return false;
    }
}

bool Database::updateNameAlbum(QString &newName,int &id){
    QSqlQuery query;
    query.prepare("UPDATE Album SET name = :name WHERE idAlbum = :idAlbum");
    query.bindValue(":name", newName);
    query.bindValue(":idAlbum", id);
    if(query.exec()){
        return true;
    }else {
        return false;
    }
}

bool Database::updateLastModifDate(int &id){
    QSqlQuery query;
    query.prepare("UPDATE Album SET lastModifDate = :lastModifDate WHERE idAlbum = :idAlbum");
    query.bindValue(":lastModifDate", QDateTime::currentDateTime());
    query.bindValue(":idAlbum", id);
    if(query.exec()){
        return true;
    }else {
        return false;
    }
}
/*********************************************************************************************/



/***************************************** IMAGE *********************************************/
int Database::addImage(QString &albumName, QString &imagePath, int score, QString &comment, QString &dominantColor, QString &feeling)
{
    Database::checkDBBeenCreated();

    // vérifi que l'image n'est pas déja presente dans l'album
    int imageId;
    if((imageId = getImageId(imagePath)) > 0){ //sinon image non présente dans la bdd
        int albumId = getAlbumId(albumName);
        if(isImageInAlbum(imageId, albumId)){
            Database::lastErrorMessage = __FUNCTION__;
            Database::lastErrorMessage.append(": L'image est déja présente dans l'album.");
            return false;
        }
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



int Database::getImageId(QString &filePath)
{
    Database::checkDBBeenCreated();

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
    Database::checkDBBeenCreated();

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
/*********************************************************************************************/


bool Database::isImageInAlbum(int imageId, int albumId)
{
    Database::checkDBBeenCreated();

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM linkImageAlbum WHERE idAlbum = :idAlbum AND idImage = :idImage;");
    query.bindValue(":idAlbum", albumId);
    query.bindValue("idImage", imageId);

    if(query.exec()){
        if(query.next() > 0)
        return true;
    } else {
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(" : Erreur lors de la requête.");
        return false;
    }
}

void Database::checkDBBeenCreated()
{
    if(Database::instance == nullptr)
        Database::lastErrorMessage = "Aucune connection à la base de données.";
}


int Database::getImagePosition(int imageId, int albumId)
{
    checkDBBeenCreated();

    QSqlQuery query;
    query.prepare("SELECT positionInAlbum FROM linkImageAlbum WHERE idAlbum = :idAlbum AND idImage = :idImage;");
    query.bindValue(":idAlbum", albumId);
    query.bindValue("idImage", imageId);

    if(query.exec()){
        qDebug() << query.value(0).toInt();
        return query.value(0).toInt();
    } else {
        QString error = __FUNCTION__;
        error.append(" : Erreur lors de la requête.");
        lastErrorMessage = error;
        return -1;
    }
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
                             positionInAlbum INTEGER,
                             CONSTRAINT pk_link PRIMARY KEY (idAlbum, idImage),
                             FOREIGN KEY (idAlbum) REFERENCES Album(idAlbum),
                             FOREIGN KEY (idImage) REFERENCES Album(idImage));

*/
