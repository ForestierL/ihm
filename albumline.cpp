#include "albumline.h"

AlbumLine::AlbumLine(QObject *parent) : QHBoxLayout()
{
    albumTitle = new QLineEdit();
    albumTitle->setToolTip("Titre de l'album");
    valideNewAlbumButton = new QHoverSensitiveButton(NULL, "validate");
    valideNewAlbumButton->setToolTip("Valider");

    albumTitle->setPlaceholderText("Titre");
    valideNewAlbumButton->setMaximumSize(20,20);

    addWidget(albumTitle);
    addWidget(valideNewAlbumButton);

    connect(getButton(), SIGNAL(clicked()), this, SLOT(emitSignalClick()));
}


void AlbumLine::setFocusToAlbumTitleLineEdit()
{
    albumTitle->setFocus();
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
