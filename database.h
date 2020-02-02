#ifndef DATABASE_H
#define DATABASE_H

#include <QFileSystemModel>
#include <QSqlDatabase>
#include <QSqlQuery>

class Database
{
public:
    Database(const QString& path);
    bool addImage(QString &filePath, int &score, QString &note, QString &color, QString &feeling);
private:
    QSqlDatabase db;
};

#endif // DATABASE_H
