#include "albumbutton.h"

AlbumButton::AlbumButton(QString name) : QHBoxLayout()
{
    albumTitle = new QPushButton();
    deleteAlbum = new QHoverSensitiveButton(NULL, "delete");

    albumTitle->setText(name);
    deleteAlbum->setMaximumSize(20,20);

    addWidget(albumTitle);
    addWidget(deleteAlbum);

    connect(getButton(), SIGNAL(clicked()), this, SLOT(emitSignalClick()));
    //connect(getLineEdit(), SIGNAL(returnPressed()), this, SLOT(emitSignalEnter()));
}




void AlbumButton::emitSignalClick()
{
    QString name = getAlbumTitleString();
    //emit this->validated(name);
}

/*
void AlbumButton::emitSignalEnter()
{
    emit this->validated(getAlbumTitleString());
}
*/


QHoverSensitiveButton* AlbumButton::getButton()
{
    return deleteAlbum;
}

QPushButton* AlbumButton::getPushButton(void)
{
    return albumTitle;
}

QString AlbumButton::getAlbumTitleString()
{
    QString name = albumTitle->text();
    return name;
}



AlbumButton::~AlbumButton()
{
    delete albumTitle;
    delete deleteAlbum;

    albumTitle = nullptr;
    deleteAlbum = nullptr;
}
