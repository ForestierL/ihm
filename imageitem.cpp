#include "imageitem.h"
#include "itemlist.h"
#include "mainwindow.h"

#include <QMenu>
#include <QSignalMapper>

QString shortText(QString text, int size = 30)
{
    QString returned;
    if(text.size() > size)
    {
        text.resize(size);
        return text+"...";
    }
    return text;
}

ImageItem::ImageItem(QWidget *parent, QString filePath, int id, bool smoothImage) : QWidget(parent)
{
    //qDebug()<< (ItemList)parent;
    this->id = id;
    QFile file(filePath);
    if(!file.exists())
    {
        return;
    }
    else
    {
        QFileInfo fileInfo(filePath);
        if(fileInfo.isDir())
        {
            isImage = false;
            createContentFolder(filePath);
        }
        else
        {
            isImage = true;
            createContentFile(filePath, true);
        }
    }
    setupLayout();
}

QString extractDirectoryName(QString path)
{
    QStringList qsl = path.split('/');
    if(qsl.size()<2)
        return path;
    qsl.removeLast(); //vide
    return qsl.last();
}

void ImageItem::createContentFolder(QString dirPath)
{
    qDebug() << "folder" << dirPath;
    QDir dir(dirPath);
    filePath = dirPath;

    //charger une image de dossier
//    QFileIconProvider::Folder
    imageLabel  = new ClickableLabel();
    //imageLabel->setStyleSheet("background-color: #7f7f7f;");
    imageLabel->setFixedSize(100,100);

    imageLabel->setAlignment(Qt::AlignCenter);
    auto icon = QFileIconProvider().icon(filePath);
    imageLabel->setPixmap(icon.pixmap(100,100));

    connect(imageLabel, SIGNAL(doubleClicked()), this, SLOT(on_dir_doubleClicked()));

    name = new QLabel(extractDirectoryName(dirPath));
    name->setStyleSheet("font-weight: bold;");
    size = new QLabel("Elements : "+QString::number(dir.count()));
    date = new QLabel();
    date->hide();
    comment = new QTextEdit();
    comment->hide();
    note = new QLabel();
    note->hide();
    color = new QLabel();
    color->hide();
    feeling = new QLabel();
    feeling->hide();
    initMover();
    disableMover(true);
}

void ImageItem::createContentFile(QString filePath, bool smoothImage)
{
    //qDebug() << "file ! " <<filePath;
    //qt.gui.icc: fromIccProfile: failed minimal tag size sanity ??? ça vient de là mais ???
    this->filePath = filePath;

    QFileInfo fileInfo(filePath);
    QImage imageInfo(filePath);

    comment = new QTextEdit();
    comment->setEnabled(true);
    comment->setReadOnly(true);
    comment->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
    note = new QLabel();
    color = new QLabel();
    feeling = new QLabel();
    name = new QLabel(shortText(fileInfo.baseName())+"."+fileInfo.completeSuffix());
    name->setStyleSheet("font-weight: bold;");
    size = new QLabel(QString::number(imageInfo.size().width()) + "x" + QString::number(imageInfo.size().height()));
    date = new QLabel(fileInfo.birthTime().toString("d/MM/yy"));
    /*Liens avec la bd
     *
     * bool loadFromDatabase(); <<<<<<<
    int idImage = Database::getImageId(filePath);
    QVector<QString> result = Database::getInfoImage(idImage);
    QString imageScore= result[0];
    QString imageComment= result[1];
    QString imageColor = result[2];
    QString imageFeeling = result[3];
    setData("",imageComment,imageScore.toInt(),imageColor,imageFeeling);
    */
    setData("","",0,"---","---");

    //Image
    imageLabel  = new ClickableLabel();
    imageLabel->setFixedSize(100,100);
    imageLabel->setAlignment(Qt::AlignCenter);
    QPixmap *pixmap_img = new QPixmap(filePath);
    Qt::TransformationMode transformationMode;
    if(smoothImage)
        transformationMode = Qt::TransformationMode::SmoothTransformation;
    else
        transformationMode = Qt::TransformationMode::FastTransformation;
    imageLabel->setPixmap(pixmap_img->scaled(100, 100, Qt::KeepAspectRatio, transformationMode));

    connect(imageLabel, SIGNAL(doubleClicked()), this, SLOT(on_ImageLabel_doubleClicked()));

    imageLabel->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(imageLabel, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ctxMenu(const QPoint &)));


    initMover();
}

void ImageItem::setupLayout()
{
    mainLayout = new QGridLayout(this);
    /*mainLayout->addWidget(imageLabel);
    mainLayout->addWidget(imageLabel, 0, 0, Qt::AlignCenter);*/
    mainLayout->addWidget(imageLabel,0,0,3,1);
    imageLabel->setToolTip(name->text());
    mainLayout->addWidget(name,     0,1,1,5);
    name->setToolTip(name->text());
    mainLayout->addWidget(size,     1,1,1,2);
    size->setToolTip("Taille du fichier");
    mainLayout->addWidget(date,     2,1,1,2);
    date->setToolTip("Date de création");
    mainLayout->addWidget(comment,  1,2,3,4);
    comment->setToolTip("Commentaires");
    mainLayout->addWidget(note,     0,6,1,1);
    note->setToolTip("Note");
    mainLayout->addWidget(color,    1,6,1,1);
    color->setToolTip("Couleur dominante");
    mainLayout->addWidget(feeling,  2,6,1,1);
    feeling->setToolTip("Emotions");
    if(isImage)
    {
    //mover
        mainLayout->addWidget(upArrow,  0,7,1,1);
        upArrow->setToolTip("Monter dans l'album");
        mainLayout->addWidget(idEdit,   1,7,1,1);
        idEdit->setDisabled(true);
        idEdit->setToolTip("Range dans l'album");
        mainLayout->addWidget(downArrow,2,7,1,1);
        downArrow->setToolTip("Descendre dans l'album");
    }
    for(int i=0; i<8; i++) {
//        mainLayout->setColumnMinimumWidth(i,size->width()/8);
        mainLayout->setColumnStretch(i, 1);
    }
    mainLayout->setHorizontalSpacing(10);
}

QString ImageItem::getFilePath() const
{
    return filePath;
}

void ImageItem::disableMover(bool disabled)
{
    if(disabled)
        this->idEdit->hide();
    else
        this->idEdit->show();
    setDisabledUp(disabled);
    setDisabledDown(disabled);
}
bool ImageItem::getIsImage() const
{
    return isImage;
}

void ImageItem::setData(QString name, QString comment, int note, QString color, QString feeling)
{
    if(!name.isEmpty())
        setName(name);
    if(!comment.isEmpty())
        setComment(comment);
    if(note != -1)
        setNote(note);
    if(!color.isEmpty())
        setColor(color);
    if(!feeling.isEmpty())
        setFeeling(feeling);
}

void ImageItem::setName(QString name)
{
    this->name->setText(name);
}
void ImageItem::setComment(QString comment)
{
    this->comment->setPlainText(comment);
}
void ImageItem::setNote(QString note)
{
    this->note->setText(note);
}
void ImageItem::setNote(int note)
{
    this->note->setText(QString::number(note)+"/5");
}
void ImageItem::setColor(QString color)
{
    this->color->setText(color);
}
void ImageItem::setFeeling(QString feeling)
{
    this->feeling->setText(feeling);
}

//from mover
void ImageItem::initMover()
{
    upArrow = new QHoverSensitiveButton(NULL,"arrow-u");
    upArrow->setFixedWidth(30);

    idEdit = new QLineEdit(QString::number(id));
    idEdit->setAlignment(Qt::AlignCenter);
    idEdit->setFixedWidth(30);
    //idEdit->hide();

    downArrow = new QHoverSensitiveButton(NULL,"arrow-d");
    downArrow->setFixedWidth(30);

    connect(upArrow, SIGNAL(clicked()), this, SLOT(move_up()));
    connect(downArrow, SIGNAL(clicked()), this, SLOT(move_down()));
}

void ImageItem::setId(int id)
{
    idEdit->setText(QString::number(id));
    this->id = id;
}

void ImageItem::setDisabledUp(bool disabled)
{
    upArrow->setDisabled(disabled);
    if(disabled)
        upArrow->hide();
    else
        upArrow->show();
}

void ImageItem::setDisabledDown(bool disabled)
{
    downArrow->setDisabled(disabled);
    if(disabled)
        downArrow->hide();
    else
        downArrow->show();
}

void ImageItem::move_up(){
}

void ImageItem::move_down(){
}


void ImageItem::ctxMenu(const QPoint &pos)
{
    QMenu myMenu;
    QPoint globalPos = this->mapToGlobal(pos);

    QAction *open = new QAction("Ouvrir", parentWidget());
    myMenu.addAction(open);
    QSignalMapper *mOpen = new QSignalMapper();
    connect(open, SIGNAL(triggered()), mOpen, SLOT(map()));
    mOpen->setMapping(open, filePath);
    connect(mOpen, SIGNAL(mapped(const QString &)), parent()->parent()->parent()->parent()->parent(), SLOT(openEditor(const QString &)));

    myMenu.addSeparator();
    QAction *add = new QAction("Ajouter à un album", parentWidget());
    myMenu.addAction(add);
    QSignalMapper *mAdd = new QSignalMapper();
    connect(add, SIGNAL(triggered()), mAdd, SLOT(map()));
    mAdd->setMapping(add, filePath);
    connect(mAdd, SIGNAL(mapped(const QString &)), parent()->parent()->parent()->parent()->parent(), SLOT(addToAlbum(const QString &)));

    QAction *info = new QAction("Informations", parentWidget());
    myMenu.addAction(info);
    QSignalMapper *mInfo = new QSignalMapper();
    connect(info, SIGNAL(triggered()), mInfo, SLOT(map()));
    mInfo->setMapping(info, filePath);
    connect(mInfo, SIGNAL(mapped(const QString &)), parent()->parent()->parent()->parent()->parent(), SLOT(informations(const QString &)));

    myMenu.addSeparator();
    QAction *erase = new QAction("Supprimer", parentWidget());
    myMenu.addAction(erase);
    QSignalMapper *mErase = new QSignalMapper();
    connect(erase, SIGNAL(triggered()), mErase, SLOT(map()));
    mErase->setMapping(erase, filePath);
    connect(mErase, SIGNAL(mapped(const QString &)), parent()->parent()->parent()->parent()->parent(), SLOT(eraseItem(const QString &)));

    myMenu.exec(globalPos);
}

void ImageItem::on_ImageLabel_doubleClicked()
{
    qobject_cast<MainWindow*>(parent())->openEditor(filePath);
}

void ImageItem::on_dir_doubleClicked()
{
    qobject_cast<MainWindow*>(parent())->updateCurrentPath(filePath);
}
