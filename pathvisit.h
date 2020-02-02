#ifndef PATHVISIT_H
#define PATHVISIT_H

#include <QString>
#include <QVector>


class PathVisit
{
public:
    PathVisit(QString currentPath);
    void addPath(QString);
    QString back();
    QString forward();
private:
    QString currentPath;
    QVector<QString> precedingPath;
    QVector<QString> followingPath;
};

#endif // PATHVISIT_H
