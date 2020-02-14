#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <QWidget>
#include <QFile>
#include <QFileInfo>
#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QDateTime>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QLineEdit>
#include <QFileIconProvider>
#include "database.h"
#include "clickablelabel.h"
#include "qhoversensitivebutton.h"

class ImageItem : public QWidget
{
    Q_OBJECT
public:
    explicit ImageItem(QWidget *parent = nullptr, QString filePath = "", int id = 0, bool smoothImages = true);
    bool loadFromDatabase();
    void setData(QString name, QString comment, int note, QString color, QString feeling);
    void setName(QString name);
    void setComment(QString comment);
    void setNote(QString note);
    void setNote(int note);
    void setColor(QString color);
    void setFeeling(QString feeling);
    bool getIsImage() const;
    QString getFilePath() const;
    void disableMover(bool disabled);

private:
    QWidget* parentGlobal;
    void createContentFile(QString filePath, bool smoothImage);
    void createContentFolder(QString dirPath);
    void setupLayout();

private:
    QGridLayout *mainLayout;
    QLabel  *imageLabel;
    bool isImage;
    QString filePath;
    QLabel *name;
    QLabel *size;
    QLabel *date;
    QTextEdit *comment;
    QLabel *note;
    QLabel *color;
    QLabel *feeling;
    //from mover
public:
    void initMover();
    void setId(int id);
    void setDisabledUp(bool disabled);
    void setDisabledDown(bool disabled);

private:
    QHoverSensitiveButton* upArrow;
    QLineEdit* idEdit;
    int id;
    QHoverSensitiveButton* downArrow;

private slots:
    void move_down();
    void move_up();
    void ctxMenu(const QPoint &pos);
    void on_ImageLabel_doubleClicked();
    void on_dir_doubleClicked();
    void deleteToAlbum();

signals:

};

#endif // IMAGEITEM_H
