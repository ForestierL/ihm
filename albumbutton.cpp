#include "albumbutton.h"

AlbumButton::AlbumButton(QString &name) : QHBoxLayout()
{
    albumTitle = new QPushButton();
    albumTitle->setToolTip(name);
    deleteAlbum = new QHoverSensitiveButton(NULL, "delete");
    deleteAlbum->setToolTip("Supprimer l'album");

    albumTitle->setText(name);
    deleteAlbum->setMaximumSize(20,20);

    addWidget(albumTitle);
    addWidget(deleteAlbum);

    connect(getButton(), SIGNAL(clicked()), this, SLOT(emitSignalClick()));
    connect(getPushButton(),SIGNAL(clicked()),this,SLOT(emitSignalOpenAlbum()));
}




void AlbumButton::emitSignalClick()
{
    emit this->validated(getAlbumTitleString());
}


void AlbumButton::emitSignalOpenAlbum()
{
    emit this->openAlbum(getAlbumTitleString());
}


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
    return albumTitle->text();
}



AlbumButton::~AlbumButton()
{
    delete albumTitle;
    delete deleteAlbum;

    albumTitle = nullptr;
    deleteAlbum = nullptr;
}
