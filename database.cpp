#include "database.h"
#include <QDebug>
#include <QDateTime>

#include <QSqlError>


int sqlQuerySize(QSqlQuery &query);

QString Database::lastErrorMessage;
Database *Database::instance;

/**
 * @brief Database::Database Constructeur de la classe.
 * @param path Chemin du fichier de base de donnees.
 */
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

/**
 * @brief Database::getInstance Créer la connection a la base de donnees seulement si aucune
 * connexion n'est établie.
 * @return Renvoi une instance de database.
 */
Database* Database::getInstance()
{
    if(Database::instance == nullptr){
        QString db_path = QDir::currentPath();
        qDebug() <<db_path;    //current path
        db_path =  db_path + QString("/lumipic.db");
        Database::instance = new Database("E:/Documents/GitHub/ihm/lumipic.db"); //todo: modifier
    }

    return Database::instance;
}

/***************************************** ALBUM *********************************************/
/**
 * @brief Database::getAlbumId Récupère en base de donnees l'identifiant unique de l'album.
 * @param name Nom de l'album.
 * @return
 */
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

/**
 * @brief Database::getAlbumsOrderByName Récupère en base de donnees la liste des noms d'albums
 * en les triant par ordre alphabetique.
 * @return Liste contenant le noms de chaque albums.
 */
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

/**
 * @brief Database::getAlbumsOrderByName Récupère en base de donnees la liste des noms d'albums
 * en les triant par date de dernière modification.
 * @return Liste contenant le noms de chaque albums.
 */
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

/**
 * @brief Database::createAlbum Crée un nouvel album en base de donnees.
 * @param name Nom du nouvel album.
 * @return True si l'ajout a réussi, false sinon.
 */
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

/**
 * @brief Database::createAlbum Supprime un album de la base de donnees.
 * @param name Nom de l'album a supprimer.
 * @return True si la suppression a réussi, false sinon.
 */
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

/**
 * @brief Database::updateNameAlbumMet a jour le nom d'un album en base de donnees.
 * @param newName Nouveau nom.
 * @param id Identifiant de l'album.
 * @return True si la modification a ete effectue, false sinon.
 */
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

/**
 * @brief Database::updateLastModifDate Met a jour la date de dernière modification d'un album.
 * @param id Album a mettre a jour.
 * @return True si la modification a ete effectue, false sinon.
 */
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
/**
 * @brief Database::addImage Ajoute une image en base de donnees.
 * @param imagePath Chemin de l'image.
 * @param score Note de l'image.
 * @param comment Commentaire de l'image.
 * @param dominantColor Couleur principale de l'image.
 * @param feeling Ressenti de l'image.
 * @return True si l'ajout a ete effectue, false sinon.
 */
bool Database::addImage(QString &imagePath, int score, QString &comment, QString &dominantColor, QString &feeling)
{
    Database::getInstance();

    int imageId;
    if((imageId = getImageId(imagePath)) > 0){// vérifi que l'image n'est pas déja presente dans la BDD
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(": Image déja présente en base de donnees.");
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

/**
 * @brief Database::addImageToAlbum Ajoute une image présente en base de donnees a un album.
 * @param imageId Identifiant de l'image.
 * @param albumId Identifiant de l'album.
 * @return True si l'ajout a ete effectue, false sinon.
 */
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

/**
 * @brief Database::getImageId Recupere l'identifiant d'une image.
 * @param filePath Chemin de l'image.
 * @return Identifiant de l'image.
 */
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
            Database::lastErrorMessage.append(": Plusieurs images correspondent au nom de fichier.");
            return -1; //ajouté par loic
        }
    } else {
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(" : Erreur lors de la requête.");
        return -1;
    }
}

/**
 * @brief Database::removeImage Supprime une image de la base de donnees.
 * @param imageId Identifiant de l'image a supprimer.
 * @return True si la suppression a ete effectuee, false sinon.
 */
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

/**
 * @brief Database::getLastImagePosition Recupere la possition de la derniere image d'un album.
 * @param albumId Identifiant de l'album.
 * @return Possition de la derniere image.
 */
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

/**
 * @brief Database::updateImage Met a jour les information d'une image en base de donnees.
 * @param idImage Identifiant de l'image a mettre a jour.
 * @param filePath Nouveau chemin.
 * @param score Nouvelle note.
 * @param comment Nouveau commentaire.
 * @param dominantColor Nouvelle couleur principale.
 * @param feeling Nouveau ressenti.
 * @return True si la modification a ete effectue, false sinon.
 */
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

    if(!query.exec())
    {
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(" : Erreur lors de la requête.");
        return false;
    }
    return true;
}

/**
 * @brief Database::updateImagePath Met a jour le chemin d'une image.
 * @param idImage Identifiant de l'image a mettre a jour.
 * @param filePath Nouveau chemin.
 * @return True si la modification a ete effectue, false sinon.
 */
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

/**
 * @brief Database::getImageFromFilter Récupere le chemin des images presentes dans un album si elles correspondent
 * aux critères.
 * @param albumId Identifiant de l'album.
 * @param color Couleur principale voulu.
 * @param feeling Ressenti voulu.
 * @param score Note voulue.
 * @return Liste des chemains d'image correspondantent aux critere.
 */
QVector<QString> Database::getImageFromFilter(int albumId, const QString &color, const QString &feeling, const QString &score)
{
    Database::getInstance();

    QVector<QString> result;

    int scoreInt = score.toInt(); //mettre une vérif sur çà

    QSqlQuery query("SELECT filePath FROM Image WHERE "
                    "score = :score AND"
                    "dominantColor = :color AND"
                    "feeling = :feeling AND"
                    "idImage IN"
                        "(SELECT idImage from linkImageAlbum WHERE"
                        "idAlbum = :idAlbum)");
    query.bindValue(":idAlbum", albumId);
    query.bindValue(":score", scoreInt);
    query.bindValue(":dominantColor", color);
    query.bindValue(":feeling", feeling);

    if(query.exec()){
        while (query.next()) {
            result.push_back(query.value(0).toString());
        }
    }

    return result;
}
/*********************************************************************************************/


/**
 * @brief Database::getAllImagePath recupere le chemin de chaques images presentes en base de donnees.
 * @return Liste des chemins.
 */
QVector<QString> Database::getAllImagePath()
{
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
/*********************************************************************************************/


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


QString Database::getImageFilePath(int idImage){
    Database::getInstance();

    QSqlQuery query;
    query.prepare("SELECT filePath FROM Image WHERE idImage = :idImage");
    query.bindValue(":idImage",idImage);

    if(query.exec()){
        query.next();
        return query.value(0).toString();
    } else {
        Database::lastErrorMessage = __FUNCTION__;
        Database::lastErrorMessage.append(" : Erreur lors de la requête.");
        return "Erreur requete";
    }
}

bool Database::updatePositionInAlbum(int imageId, int albumId, int newPosition){

    Database::getInstance();

    QSqlQuery query;
    query.prepare("UPDATE linkImageAlbum SET positionInAlbum = :positionInAlbum WHERE idAlbum = :idAlbum AND idImage = :idImage");
    query.bindValue(":positionInAlbum", newPosition);
    query.bindValue(":idImage", imageId);
    query.bindValue(":idAlbum", albumId);

    if(query.exec())
    {
        qDebug() << "EXEC !!!!!!!!!!!";
        return true;
    }
    else
    {
        Database::lastErrorMessage.append(" : Erreur lors de la requête.");
        return false;
    }
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
