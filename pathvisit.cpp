#include "pathvisit.h"
#include <QDebug>

PathVisit::PathVisit(QString currentPath)
{
    this->currentPath = currentPath;
    precedingPath.clear(); //ceux passés
    followingPath.clear(); //ceux qu'on a passé
    qDebug() << "PathVisit >> PathVisit >> " + currentPath;
}

//on ajoute un path (clic dessus un dossier par exemple)
void PathVisit::addPath(QString path)
{
    //si on clique plusieurs fois sur le dossier (comme un boomer)
    if(currentPath == path)
        return;
    if(precedingPath.isEmpty())
    {
        precedingPath.append(currentPath);
        followingPath.clear();
        currentPath = path;
        qDebug() << "PathVisit >> addPath >> " + path;
    }
    else if(currentPath != precedingPath.last())
    {
        precedingPath.append(currentPath);
        followingPath.clear();
        currentPath = path;
        qDebug() << "PathVisit >> addPath >> " + path;
    }
}

QString PathVisit::back()
{
    if(precedingPath.length() == 0)
        return currentPath;
    followingPath.append(currentPath);
    currentPath = precedingPath.last();
    precedingPath.removeLast();
    qDebug() << "PathVisit >> back >> " + currentPath;
    return currentPath;
}

QString PathVisit::forward()
{
    if(followingPath.length() == 0)
        return currentPath;
    precedingPath.append(currentPath);
    currentPath = followingPath.last();
    followingPath.removeLast();
    qDebug() << "PathVisit >> forward >> " + currentPath;
    return currentPath;
}

QString PathVisit::getCurrentPath(){
    return currentPath;
}
