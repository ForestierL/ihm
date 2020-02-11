#include "editionwindow.h"
#include "ui_editionwindow.h"

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
#include <QtMath>


EditionWindow::EditionWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::EditionWindow)
{
    ui->setupUi(this);

    QFrame *toolBarFrame = createToolBar();
    ui->toolBar->insertWidget(0, toolBarFrame);
    ui->toolBar->setMovable(false);

    QFrame *statusBarFrame = createStatusBar();
    ui->statusbar->insertPermanentWidget(0, statusBarFrame, 1);

    initBackground();
}


QFrame* EditionWindow::createToolBar(void)
{
    /********* Composant de la tool bar *********/
    QPushButton *returnButton = new QPushButton("a");
    QPushButton *repeatButton = new QPushButton("b");

    QPushButton *rotateButton = new QPushButton("c");
    connect(rotateButton, SIGNAL(clicked()), this, SLOT(rotateImage()));
    QPushButton *horizontalMirrorButton = new QPushButton("d");
    connect(horizontalMirrorButton, SIGNAL(clicked()), this, SLOT(horizontalMirror()));
    QPushButton *verticalMirrorButton = new QPushButton("e");
    connect(verticalMirrorButton, SIGNAL(clicked()), this, SLOT(verticalMirror()));
    QPushButton *trimButton = new QPushButton("f");
    connect(trimButton, SIGNAL(clicked()), this, SLOT(cropImage()));
    QPushButton *resizeButton = new QPushButton("g");

    QSlider *tempSlider = new QSlider();
    tempSlider->setFixedSize(150, 20);
    tempSlider->setOrientation(Qt::Horizontal);
    tempSlider->setMinimum(0);
    tempSlider->setMaximum(5);
    tempSlider->setTickInterval(1);
    tempSlider->setValue(0);

    QLabel *tempLabel = new QLabel("0");
    tempLabel->setFixedSize(8, 20);
    QLabel *temp2Label = new QLabel("/ 5");
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
    hBoxLayout->addWidget(repeatButton);
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
    QLabel *addToAlbumLabel = new QLabel("Ajouter à un album");

    QSlider *zoomSlider = new QSlider();
    zoomSlider->setFixedSize(150, 20);
    zoomSlider->setOrientation(Qt::Horizontal);
    zoomSlider->setMinimum(1);
    zoomSlider->setMaximum(200);
    zoomSlider->setTickInterval(1);
    zoomSlider->setValue(100);

    QLabel *percentZoomLabel = new QLabel("100");
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
    QImageReader *reader = new QImageReader(this->imagePath);
    reader->setAutoTransform(true);
    QImage srcImage(this->imagePath);
    newImage = srcImage;

    if (srcImage.isNull()) {
        QMessageBox::information(this,
                                 QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2").arg(QDir::toNativeSeparators(this->imagePath),
                                                              reader->errorString()));
    }
    else{
        delete reader;
        reader = nullptr;

        QImage image(srcImage.width(), srcImage.height(), srcImage.format());
        dstImage = image;
        QPainter painter(&dstImage);
        painter.drawImage(0, 0, srcImage);

        QLabel *imageLabel = new QLabel();
        imageLabel->setPixmap(QPixmap::fromImage(dstImage));

        this->imageLabel = imageLabel;

        this->initialPixMap = QPixmap::fromImage(dstImage);
        this->initialImageWidth = imageLabel->pixmap()->width();
        this->initialImageHeigth = imageLabel->pixmap()->height();
        actualImageWidth = initialImageWidth;
        actualImageHeigth = initialImageHeigth;

        QHBoxLayout *contentLayout = new QHBoxLayout();
        contentLayout->addStretch(1);
        contentLayout->addWidget(imageLabel);
        contentLayout->addStretch(1);

        this->ui->centralwidget->setLayout(contentLayout);
    }
}

void EditionWindow::resizeImage(int percent)
{
    actualImageWidth = this->initialImageWidth * percent/100;
    actualImageHeigth = this->initialImageHeigth * percent/100;

    this->imageLabel->setPixmap(this->initialPixMap.scaled(actualImageWidth, actualImageHeigth, Qt::KeepAspectRatio));
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

void EditionWindow::cropImage(){
    crop=true;
    QPainter painter(&dstImage);

    imageLabel->setPixmap(QPixmap::fromImage(dstImage).scaled(actualImageWidth, actualImageHeigth, Qt::KeepAspectRatio));
}

void EditionWindow::mousePressEvent(QMouseEvent *event){
    if(crop){
        QPoint p = imageLabel->mapFromGlobal(QCursor::pos());

        float px = p.rx()*(initialImageWidth/actualImageWidth);
        float py = (p.ry()+(-centralWidget()->height()+actualImageHeigth)/2)*(initialImageHeigth/actualImageHeigth);

        rect.setCoords(px,py,px,py);
        QPainter painter(&dstImage);
        painter.drawImage(0,0, newImage);
        QBrush brush(Qt::cyan, Qt::Dense6Pattern);
        painter.drawRect(rect);
        painter.fillRect(rect, brush);
        imageLabel->setPixmap(QPixmap::fromImage(dstImage).scaled(actualImageWidth, actualImageHeigth, Qt::KeepAspectRatio));
    }
}

void EditionWindow::mouseMoveEvent(QMouseEvent *event){
    if(crop){
        QPoint p = imageLabel->mapFromGlobal(QCursor::pos());

        float px = p.rx()*(initialImageWidth/actualImageWidth);
        float py = (p.ry()+(-centralWidget()->height()+actualImageHeigth)/2)*(initialImageHeigth/actualImageHeigth);
        rect.setCoords(rect.topLeft().rx(), rect.topLeft().ry(), px, py);

        QPainter painter(&dstImage);
        painter.drawRect(rect);
//        QImage image(imagePath);
//        newImage = image;
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

            QPixmap cropped = this->initialPixMap.copy(rect);

            initialPixMap = cropped;
            newImage = cropped.toImage();
            dstImage = newImage;
            initialImageWidth = newImage.width();
            initialImageHeigth = newImage.height();
            actualImageWidth = initialImageWidth;
            actualImageHeigth = initialImageHeigth;

            this->imageLabel->setPixmap(initialPixMap);
        }
        crop = false;
    }
}

void EditionWindow::verticalMirror(){
    QPainter painter(&dstImage);
    newImage = newImage.mirrored();
    painter.drawImage(0,0,newImage);
    initialPixMap = QPixmap::fromImage(newImage);
    imageLabel->setPixmap(QPixmap::fromImage(dstImage).scaled(actualImageWidth, actualImageHeigth, Qt::KeepAspectRatio));
}

void EditionWindow::horizontalMirror(){
    QPainter painter(&dstImage);
    newImage = newImage.mirrored(true, false);
    painter.drawImage(0,0,newImage);
    initialPixMap = QPixmap::fromImage(newImage);
    imageLabel->setPixmap(QPixmap::fromImage(dstImage).scaled(actualImageWidth, actualImageHeigth, Qt::KeepAspectRatio));
}

void EditionWindow::rotateImage(){
    QPainter painter(&dstImage);
    QTransform trans;
    newImage = newImage.transformed(trans.rotate(90));
    initialPixMap = QPixmap::fromImage(newImage);
    painter.drawImage(0,0,newImage);
    painter.end();
    dstImage = newImage;
    initialImageWidth = newImage.width();
    initialImageHeigth = newImage.height();
    actualImageWidth = initialImageWidth;
    actualImageHeigth = initialImageHeigth;

    this->imageLabel->setPixmap(this->initialPixMap.scaled(actualImageWidth, actualImageHeigth, Qt::KeepAspectRatio));
}

EditionWindow::~EditionWindow()
{
    delete ui;
}
