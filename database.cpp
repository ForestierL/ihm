#include "database.h"
#include <QDebug>

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
//  qDebug() << query.exec("INSERT into Image(0, 'filePath/lalala.png', 4, 'Ceci est une note', '0,255,255', 'Dynamique')");
    /*qDebug() << query.exec("SELECT * FROM Image");
    qDebug() << query.isNull(0);
    qDebug() << query.isSelect();
    qDebug() << query.isValid();*/
}

bool Database::addImage(QString &filePath, int &score, QString &note, QString &dominantColor, QString &feeling)
{
     qDebug() << "add image";

    bool success = false;

    QSqlQuery query;
    /*query.prepare("INSERT into Image(0, filePath(:filePath), score(:score), note(:note), dominantColor(:dominantColor), feeling(:feeling))");
    query.bindValue(":filePath", filePath);
    query.bindValue(":score", score);
    query.bindValue(":note", note);
    query.bindValue(":dominantColor", dominantColor);
    query.bindValue(":feeling", feeling);*/

    query.prepare("INSERT into Image(0, '" + filePath + "', " + QString::number(score) + ", '" + note + "', '" + dominantColor + "', '" + feeling + "')");

    if(query.exec())
    {
        success = true;
    }
    else
    {
         qDebug() << "add image error";
    }

    return success;
}
