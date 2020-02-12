#ifndef ALBUMLINE_H
#define ALBUMLINE_H

#include <QObject>
#include <QLineEdit>
#include <qhoversensitivebutton.h>

class AlbumLine : public QHBoxLayout
{
    Q_OBJECT
public:
    explicit AlbumLine(QObject *parent = nullptr);
    ~AlbumLine();

    QHoverSensitiveButton* getButton(void);
    QLineEdit* getLineEdit(void);
    QString getAlbumTitleString(void);

    void setFocusToAlbumTitleLineEdit();


private:
    QLineEdit *albumTitle;
    QHoverSensitiveButton *valideNewAlbumButton;


private slots:
    void emitSignalClick(void);
    //void emitSignalEnter(void);

signals:
    void validated(const QString &albumName);


}; //class AlbumLine

#endif // ALBUMLINE_H
