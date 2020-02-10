#ifndef ALBUMBUTTON2_H
#define ALBUMBUTTON2_H

#include <QObject>
#include <QHBoxLayout>
#include "qhoversensitivebutton.h"
#include <QLineEdit>

class AlbumButton : public QHBoxLayout
{
    Q_OBJECT
public:
    explicit AlbumButton(QString &name);
    ~AlbumButton();

    QHoverSensitiveButton* getButton(void);
    QPushButton* getPushButton(void);
    QString getAlbumTitleString(void);


private:
    QPushButton *albumTitle;
    QHoverSensitiveButton *deleteAlbum;


private slots:
    void emitSignalClick(void);
    void emitSignalOpenAlbum(void);

signals:
    void validated(const QString &albumName);
    void openAlbum(const QString &albumName);

};

#endif // ALBUMBUTTON2_H
