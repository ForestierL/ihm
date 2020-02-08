#include "albumline.h"

AlbumLine::AlbumLine(QObject *parent) : QHBoxLayout()
{
    albumTitle = new QLineEdit();
    valideNewAlbumButton = new QHoverSensitiveButton(NULL, "ok");

    albumTitle->setPlaceholderText("Titre");
    valideNewAlbumButton->setMaximumSize(20,20);

    addWidget(albumTitle);
    addWidget(valideNewAlbumButton);

    connect(getButton(), SIGNAL(clicked()), this, SLOT(emitSignalClick()));
    //connect(getLineEdit(), SIGNAL(returnPressed()), this, SLOT(emitSignalEnter()));
}




void AlbumLine::emitSignalClick()
{
    emit this->validated(getAlbumTitleString());
}

/*
void AlbumLine::emitSignalEnter()
{
    emit this->validated(getAlbumTitleString());
}
*/


QHoverSensitiveButton* AlbumLine::getButton()
{
    return valideNewAlbumButton;
}

QLineEdit* AlbumLine::getLineEdit(void)
{
    return albumTitle;
}

QString AlbumLine::getAlbumTitleString()
{
    return albumTitle->text();
}



AlbumLine::~AlbumLine()
{
    delete albumTitle;
    delete valideNewAlbumButton;

    albumTitle = nullptr;
    valideNewAlbumButton = nullptr;
}
