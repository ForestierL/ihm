#ifndef ALBUMBUTTON_H
#define ALBUMBUTTON_H

#include <QLineEdit>
#include <qhoversensitivebutton.h>

class AlbumButton : public QHBoxLayout
{
public:
    explicit AlbumButton(QString name);
    ~AlbumButton();

    QHoverSensitiveButton* getButton(void);
    QPushButton* getPushButton(void);
    QString getAlbumTitleString(void);


private:
    QPushButton *albumTitle;
    QHoverSensitiveButton *deleteAlbum;


private slots:
    void emitSignalClick(void);
    //void emitSignalEnter(void);

signals:
    void validated(QString albumName);


}; //class AlbumButton

#endif // ALBUMBUTTON_H
