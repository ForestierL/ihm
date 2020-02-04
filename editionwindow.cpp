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
    QPushButton *horizontalMirrorButton = new QPushButton("d");
    QPushButton *verticalMirrorButton = new QPushButton("e");
    QPushButton *trimButton = new QPushButton("f");
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

    if (srcImage.isNull()) {
        QMessageBox::information(this,
                                 QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2").arg(QDir::toNativeSeparators(this->imagePath),
                                                              reader->errorString()));
    }
    else {
        delete reader;
        reader = nullptr;

        QImage dstImage(srcImage.width(), srcImage.height(), srcImage.format());

        QPainter painter(&dstImage);
        painter.drawImage(0, 0, srcImage);

        QLabel *imageLabel = new QLabel();
        imageLabel->setPixmap(QPixmap::fromImage(dstImage));

        this->imageLabel = imageLabel;

        this->initialPixMap = QPixmap::fromImage(dstImage);
        this->initialImageWidth = imageLabel->pixmap()->width();
        this->initialImageHeigth = imageLabel->pixmap()->height();

        QHBoxLayout *contentLayout = new QHBoxLayout();
        contentLayout->addStretch(1);
        contentLayout->addWidget(imageLabel);
        contentLayout->addStretch(1);

        this->ui->centralwidget->setLayout(contentLayout);
    }
}


void EditionWindow::resizeImage(int percent)
{
    float newWidth = this->initialImageWidth * percent/100;
    float newHeigth = this->initialImageHeigth * percent/100;

    this->imageLabel->setPixmap(this->initialPixMap.scaled(newWidth, newHeigth, Qt::KeepAspectRatio));
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


EditionWindow::~EditionWindow()
{
    delete ui;
}
