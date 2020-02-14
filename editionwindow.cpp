#include "editionwindow.h"
#include "ui_editionwindow.h"
#include "themeapplier.h"
#include "qhoversensitivebutton.h"
#include "resizewindow.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QSlider>
#include <QDebug>
#include <QImageReader>
#include <QMessageBox>
#include <QDir>
#include <QPainter>
#include <QGraphicsScene>
#include <QPixmap>
#include <QtMath>


EditionWindow::EditionWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::EditionWindow)
{
    ui->setupUi(this);

    windowWidth = this->width();
    windowHeigth = this->height();

    QFrame *toolBarFrame = createToolBar();
    ui->toolBar->insertWidget(0, toolBarFrame);
    ui->toolBar->setMovable(false);

    QFrame *statusBarFrame = createStatusBar();
    ui->statusbar->insertPermanentWidget(0, statusBarFrame, 1);

    initBackground();
    initConnects();
}

void EditionWindow::showEvent(QShowEvent* event){
    QMainWindow::showEvent(event);
    new themeApplier(*this);
}

QFrame* EditionWindow::createToolBar(void)
{
    /********* Composant de la tool bar *********/
    QHoverSensitiveButton *returnButton = new QHoverSensitiveButton(this, "previous");
    returnButton->setToolTip("Annuler toutes les modifications");
    connect(returnButton, SIGNAL(clicked()), this, SLOT(resetImage()));

    QHoverSensitiveButton *rotateButton = new QHoverSensitiveButton(this, "rotate");
    rotateButton->setToolTip("Pivoter l'image de 90°");
    connect(rotateButton, SIGNAL(clicked()), this, SLOT(rotateImage()));

    QHoverSensitiveButton *horizontalMirrorButton = new QHoverSensitiveButton(this, "mirror-h");
    horizontalMirrorButton->setToolTip("Miroir horizontal");
    connect(horizontalMirrorButton, SIGNAL(clicked()), this, SLOT(horizontalMirror()));

    QHoverSensitiveButton *verticalMirrorButton = new QHoverSensitiveButton(this, "mirror-v");
    verticalMirrorButton->setToolTip("Miroir vertical");
    connect(verticalMirrorButton, SIGNAL(clicked()), this, SLOT(verticalMirror()));

    QHoverSensitiveButton *trimButton = new QHoverSensitiveButton(this, "crop");
    trimButton->setToolTip("Rogner");
    connect(trimButton, SIGNAL(clicked()), this, SLOT(cropImage()));

    QHoverSensitiveButton *resizeButton = new QHoverSensitiveButton(this, "resize");
    resizeButton->setToolTip("Redimenssionner");
    connect(resizeButton, SIGNAL(clicked()), this, SLOT(resizeWindow()));


    QSlider *tempSlider = new QSlider();
    tempSlider->setFixedSize(150, 20);
    tempSlider->setOrientation(Qt::Horizontal);
    tempSlider->setMinimum(0);
    tempSlider->setMaximum(5);
    tempSlider->setTickInterval(1);
    tempSlider->setValue(0);
    tempSlider->setToolTip("Note");

    QLabel *tempLabel = new QLabel("0");
    tempLabel->setToolTip("Note");
    tempLabel->setFixedSize(8, 20);
    QLabel *temp2Label = new QLabel("/ 5");
    temp2Label->setToolTip("Note");
    temp2Label->setFixedSize(20, 20);

    QHBoxLayout *tempLayout = new QHBoxLayout();
    tempLayout->addWidget(tempSlider);
    tempLayout->addStretch(1);
    tempLayout->addWidget(tempLabel);
    tempLayout->addWidget(temp2Label);

    connect(tempSlider, SIGNAL(valueChanged(int)), tempLabel, SLOT(setNum(int)));
    /********************************************/

    QFrame *toolBarFrame = new QFrame();
    toolBarFrame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    toolBarFrame->setLineWidth(0);

    QHBoxLayout *hBoxLayout = new QHBoxLayout(toolBarFrame);
    hBoxLayout->setContentsMargins(0, 0, 0, 0);

    hBoxLayout->addWidget(returnButton);
    //hBoxLayout->addWidget(repeatButton);
    hBoxLayout->addStretch(1);
    hBoxLayout->addWidget(rotateButton);
    hBoxLayout->addWidget(horizontalMirrorButton);
    hBoxLayout->addWidget(verticalMirrorButton);
    hBoxLayout->addWidget(trimButton);
    hBoxLayout->addWidget(resizeButton);
    hBoxLayout->addStretch(1);
    hBoxLayout->addLayout(tempLayout);

    return toolBarFrame;
}


QFrame* EditionWindow::createStatusBar(void)
{
    /******** Composant de la status bar ********/
    QPushButton *addToAlbumButton = new QPushButton("+");
    addToAlbumButton->setToolTip("Ajouter à un album");
    QLabel *addToAlbumLabel = new QLabel("Ajouter à un album");

    zoomSlider = new QSlider();
    zoomSlider->setFixedSize(150, 20);
    zoomSlider->setOrientation(Qt::Horizontal);
    zoomSlider->setMinimum(1);
    zoomSlider->setMaximum(200);
    zoomSlider->setTickInterval(1);
    zoomSlider->setValue(100);
    zoomSlider->setToolTip("Zoomer");

    QLabel *percentZoomLabel = new QLabel("100");
    percentZoomLabel->setToolTip("Niveau de zoom");
    QLabel *percentLabel = new QLabel("%");

    QHBoxLayout *zoomLayout = new QHBoxLayout();
    zoomLayout->addWidget(percentZoomLabel);
    zoomLayout->addWidget(percentLabel);
    zoomLayout->addStretch(1);
    zoomLayout->addWidget(zoomSlider);

    connect(zoomSlider, SIGNAL(valueChanged(int)), percentZoomLabel, SLOT(setNum(int)));
    connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(resizeImage(int)));
    /********************************************/

    QFrame *statusFrame = new QFrame();
    statusFrame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    statusFrame->setLineWidth(0);

    QHBoxLayout *hBoxLayout = new QHBoxLayout(statusFrame);
    hBoxLayout->setContentsMargins(0, 0, 0, 0);

    hBoxLayout->addWidget(addToAlbumButton);
    hBoxLayout->addWidget(addToAlbumLabel);
    hBoxLayout->addStretch(1);
    hBoxLayout->addLayout(zoomLayout);

    return statusFrame;
}


void EditionWindow::createContents()
{
    QImageReader *reader = new QImageReader(imagePath);
    reader->setAutoTransform(true);
    QImage srcImage = reader->read();
    this->setWindowTitle(QString("LumiPix - %1").arg(imagePath));
    dstImage = srcImage;

    if (srcImage.isNull()) {
        QMessageBox::information(this,
                                 QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2").arg(QDir::toNativeSeparators(this->imagePath),
                                                              reader->errorString()));
    }
    else{
        delete reader;
        reader = nullptr;

        QLabel *imageLabel = new QLabel();
        imageLabel->setPixmap(QPixmap::fromImage(srcImage));

        this->imageLabel = imageLabel;

        actualImageWidth = srcImage.width();
        actualImageHeigth = srcImage.height();

        if(windowWidth < dstImage.width() || windowHeigth < dstImage.height()){
            zoomSlider->setValue((100 * windowWidth) / srcImage.width());
            imageLabel->setPixmap(QPixmap::fromImage(dstImage).scaled(windowWidth, windowHeigth, Qt::KeepAspectRatio));
        }

        QHBoxLayout *contentLayout = new QHBoxLayout();
        contentLayout->addStretch(1);
        contentLayout->addWidget(imageLabel);
        contentLayout->addStretch(1);
        this->ui->centralwidget->setLayout(contentLayout);
    }
}

void EditionWindow::resizeImage(int percent)
{
    actualImageWidth = dstImage.width() * percent/100;
    actualImageHeigth = dstImage.height() * percent/100;

    imageLabel->setPixmap(QPixmap::fromImage(dstImage).scaled(actualImageWidth, actualImageHeigth, Qt::KeepAspectRatio));
}

void EditionWindow::initConnects(){
    connect(ui->saveAct, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));
}

void EditionWindow::initBackground()
{
    QColor color;
    color.setRgb(99, 99, 99);

    QPalette palette;
    palette.setColor(QPalette::Window, color);

    setPalette(palette);
}

void EditionWindow::setImage(const QString &fileName)
{
    this->imagePath = fileName;
}

void EditionWindow::save()
{
    QImage finalImage = imageLabel->pixmap()->toImage();
    QFile initiaImageFile(imagePath);

    if(!initiaImageFile.remove()){
        QMessageBox::information(this, "Erreur", "Erreur lors de la sauvegarde l'image1");
        return;
    }

    if(!finalImage.save(imagePath)){
        QMessageBox::information(this, "Erreur", "Erreur lors de la sauvegarde l'image2");
    }
}

void EditionWindow::saveAs()
{
    QString filename = QFileDialog::getSaveFileName(this);
    qDebug() << "FileName: " << filename;

    QImage finalImage = imageLabel->pixmap()->toImage();

    QStringList list = imagePath.split(".");
    QString finalName = filename;
    finalName.append('.');
    finalName.append(list.last());

    if(!finalImage.save(finalName)){
        QMessageBox::information(this, "Erreur", "Erreur lors de la sauvegarde l'image");
    }
}

// **************** Modifications ******************//

void EditionWindow::cropImage(){
    crop=true;
    QPainter painter(&dstImage);

    imageLabel->setPixmap(QPixmap::fromImage(dstImage).scaled(actualImageWidth, actualImageHeigth, Qt::KeepAspectRatio));
}

void EditionWindow::mousePressEvent(QMouseEvent *event){
    if(crop){
        QPoint p = imageLabel->mapFromGlobal(QCursor::pos());

        float px = p.rx()*(dstImage.width()/actualImageWidth);
        float py = (p.ry()+(-centralWidget()->height()+actualImageHeigth)/2)*(dstImage.height()/actualImageHeigth);

        newImage = dstImage;
        rect.setCoords(px,py,px,py);
        QPainter painter(&dstImage);
        painter.drawImage(0,0, dstImage);
        QBrush brush(Qt::cyan, Qt::Dense4Pattern);
        painter.drawRect(rect);
        painter.fillRect(rect, brush);
        imageLabel->setPixmap(QPixmap::fromImage(dstImage).scaled(actualImageWidth, actualImageHeigth, Qt::KeepAspectRatio));
    }
}

void EditionWindow::mouseMoveEvent(QMouseEvent *event){
    if(crop){
        QPoint p = imageLabel->mapFromGlobal(QCursor::pos());

        float px = p.rx()*(dstImage.width()/actualImageWidth);
        float py = (p.ry()+(-centralWidget()->height()+actualImageHeigth)/2)*(dstImage.height()/actualImageHeigth);
        rect.setCoords(rect.topLeft().rx(), rect.topLeft().ry(), px, py);

        QPainter painter(&dstImage);
        painter.drawRect(rect);
        painter.drawImage(0,0, newImage);
        QBrush brush(Qt::cyan, Qt::Dense6Pattern);
        painter.drawRect(rect);
        painter.fillRect(rect, brush);
        imageLabel->setPixmap(QPixmap::fromImage(dstImage).scaled(actualImageWidth, actualImageHeigth, Qt::KeepAspectRatio));
    }
}

void EditionWindow::mouseReleaseEvent(QMouseEvent *event){
    if(crop){
        int reponse = QMessageBox::question(this, "Rogner", "Êtes-vous sûr de vouloir rogner cette image ?", QMessageBox ::Yes | QMessageBox::No);
        if (reponse == QMessageBox::Yes)
        {
            rect.setCoords(qMin(rect.topLeft().rx(),rect.bottomRight().rx()), qMin(rect.topLeft().ry(),rect.bottomRight().ry()), qMax(rect.topLeft().rx(),rect.bottomRight().rx()), qMax(rect.topLeft().ry(),rect.bottomRight().ry()));
            if(rect.topLeft().rx() < 0)
                rect.setCoords(0, rect.topLeft().ry(), rect.bottomRight().rx(), rect.bottomRight().ry());
            if(rect.topLeft().ry() < 0)
                rect.setCoords(rect.topLeft().rx(), 0, rect.bottomRight().rx(), rect.bottomRight().ry());
            if(rect.bottomRight().rx() > dstImage.width())
                rect.setCoords(0, rect.topLeft().ry(), dstImage.width(), rect.bottomRight().ry());
            if(rect.bottomRight().ry() > dstImage.height())
                rect.setCoords(rect.topLeft().rx(), rect.topLeft().ry(), rect.bottomRight().rx(), dstImage.height());
            dstImage = newImage.copy(rect);
            actualImageHeigth = dstImage.height() * zoomSlider->value()/100;
            actualImageWidth = dstImage.width() * zoomSlider->value()/100;
        }
        else{
            dstImage = newImage;
        }
        crop = false;
        imageLabel->setPixmap(QPixmap::fromImage(dstImage).scaled(actualImageWidth, actualImageHeigth, Qt::KeepAspectRatio));
    }
}

void EditionWindow::verticalMirror(){
    crop = false;
    QPainter painter(&dstImage);
    newImage = dstImage.mirrored(true, false);
    painter.drawImage(0,0,newImage);
    painter.end();
    dstImage = newImage;
    actualImageHeigth = dstImage.height() * zoomSlider->value()/100;
    actualImageWidth = dstImage.width() * zoomSlider->value()/100;
    imageLabel->setPixmap(QPixmap::fromImage(dstImage).scaled(actualImageWidth, actualImageHeigth, Qt::KeepAspectRatio));
}

void EditionWindow::horizontalMirror(){
    crop = false;
    QPainter painter(&dstImage);
    newImage = dstImage.mirrored();
    painter.drawImage(0,0,newImage);
    painter.end();
    dstImage = newImage;
    actualImageHeigth = dstImage.height() * zoomSlider->value()/100;
    actualImageWidth = dstImage.width() * zoomSlider->value()/100;
    imageLabel->setPixmap(QPixmap::fromImage(dstImage).scaled(actualImageWidth, actualImageHeigth, Qt::KeepAspectRatio));
}

void EditionWindow::rotateImage(){
    crop = false;
    QPainter painter(&dstImage);
    QTransform trans;
    QImage newImage = dstImage.transformed(trans.rotate(90));
    painter.drawImage(0,0,newImage);
    painter.end();
    dstImage = newImage;
    actualImageHeigth = dstImage.height() * zoomSlider->value()/100;
    actualImageWidth = dstImage.width() * zoomSlider->value()/100;
    imageLabel->setPixmap(QPixmap::fromImage(dstImage).scaled(actualImageWidth, actualImageHeigth, Qt::KeepAspectRatio));
}

void EditionWindow::resizeWindow(){
    ResizeWindow r(dstImage, this);
    r.show();
    QEventLoop eventLoop;
    eventLoop.exec();
}

void EditionWindow::resizePhoto(const int p){
    qDebug() << p;
    QImage img = dstImage.scaled(dstImage.width()*p/100, dstImage.height()*p/100, Qt::KeepAspectRatio);
    QPainter painter(&dstImage);
    painter.drawImage(0, 0, img);
    painter.end();
    dstImage = img;
    actualImageHeigth = dstImage.height() * zoomSlider->value()/100;
    actualImageWidth = dstImage.width() * zoomSlider->value()/100;
    imageLabel->setPixmap(QPixmap::fromImage(dstImage).scaled(actualImageWidth, actualImageHeigth, Qt::KeepAspectRatio));
}

void EditionWindow::resetImage(){
    QPainter painter(&dstImage);
    QImageReader *reader = new QImageReader(imagePath);
    reader->setAutoTransform(true);
    QImage srcImage = reader->read();
    painter.drawImage(0, 0, srcImage);
    painter.end();
    dstImage = srcImage;

    zoomSlider->setValue((100 * windowWidth) / dstImage.width());
    actualImageWidth = srcImage.width() * zoomSlider->value()/100;
    actualImageHeigth = srcImage.height() * zoomSlider->value()/100;
    imageLabel->setPixmap(QPixmap::fromImage(dstImage).scaled(actualImageWidth, actualImageHeigth, Qt::KeepAspectRatio));
}

EditionWindow::~EditionWindow()
{
    delete ui;
    delete imageLabel;
    delete zoomSlider;

    ui = nullptr;
    imageLabel = nullptr;
    zoomSlider = nullptr;
}
