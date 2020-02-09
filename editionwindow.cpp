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
#include <QFileDialog>


EditionWindow::EditionWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::EditionWindow)
{
    ui->setupUi(this);

    _window_width = parent->width();
    _window_heigth = parent->height();

    QFrame *toolBarFrame = create_tool_bar();
    ui->toolBar->insertWidget(0, toolBarFrame);
    ui->toolBar->setMovable(false);

    QFrame *statusBarFrame = create_status_bar();
    ui->statusbar->insertPermanentWidget(0, statusBarFrame, 1);

    init_background();

    init_connects();
}


QFrame* EditionWindow::create_tool_bar(void)
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


QFrame* EditionWindow::create_status_bar(void)
{
    /******** Composant de la status bar ********/
    QPushButton *addToAlbumButton = new QPushButton("+");
    QLabel *addToAlbumLabel = new QLabel("Ajouter à un album");

    _zoom_slider = new QSlider();
    _zoom_slider->setFixedSize(150, 20);
    _zoom_slider->setOrientation(Qt::Horizontal);
    _zoom_slider->setMinimum(1);
    _zoom_slider->setMaximum(200);
    _zoom_slider->setTickInterval(1);
    _zoom_slider->setValue(100);

    QLabel *percentZoomLabel = new QLabel("100");
    QLabel *percentLabel = new QLabel("%");

    QHBoxLayout *zoomLayout = new QHBoxLayout();
    zoomLayout->addWidget(percentZoomLabel);
    zoomLayout->addWidget(percentLabel);
    zoomLayout->addStretch(1);
    zoomLayout->addWidget(_zoom_slider);

    connect(_zoom_slider, SIGNAL(valueChanged(int)), percentZoomLabel, SLOT(setNum(int)));
    connect(_zoom_slider, SIGNAL(valueChanged(int)), this, SLOT(resize_image(int)));
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


void EditionWindow::create_contents()
{
    QImageReader *reader = new QImageReader(this->_image_path);
    reader->setAutoTransform(true);

    QImage srcImage = reader->read();

    if (srcImage.isNull()) {
        QMessageBox::information(this,
                                 QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2").arg(QDir::toNativeSeparators(this->_image_path),
                                                              reader->errorString()));
    }
    else {
        QLabel *imageLabel = new QLabel();
        imageLabel->setPixmap(QPixmap::fromImage(srcImage));

        this->_image_label = imageLabel;
        this->_initial_image = srcImage;

        if(_window_width < _initial_image.width() || _window_heigth < _initial_image.height()){
        _zoom_slider->setValue((100 * _window_width) / srcImage.width());
        _image_label->setPixmap(QPixmap::fromImage(_initial_image).scaled(_window_width, _window_heigth, Qt::KeepAspectRatio));
        }

        QHBoxLayout *contentLayout = new QHBoxLayout();
        contentLayout->addStretch(1);
        contentLayout->addWidget(imageLabel);
        contentLayout->addStretch(1);

        this->ui->centralwidget->setLayout(contentLayout);
    }

    delete reader;
    reader = nullptr;
}


void EditionWindow::init_background()
{
    QColor color;
    color.setRgb(99, 99, 99);

    QPalette palette;
    palette.setColor(QPalette::Window, color);

    setPalette(palette);
}


void EditionWindow::init_connects(){
    connect(ui->saveAct, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->saveAsAct, SIGNAL(triggered()), this, SLOT(save_as()));
}


void EditionWindow::set_image(const QString &fileName)
{
    this->_image_path = fileName;
}


EditionWindow::~EditionWindow()
{
    delete ui;
    delete _image_label;
    delete _zoom_slider;

    ui = nullptr;
    _image_label = nullptr;
    _zoom_slider = nullptr;
}


/************************************* SLOT *************************************/
void EditionWindow::resize_image(int percent)
{
    float newWidth = this->_initial_image.width() * percent/100;
    float newHeigth = this->_initial_image.height() * percent/100;

    this->_image_label->setPixmap(QPixmap::fromImage(_initial_image).scaled(newWidth, newHeigth, Qt::KeepAspectRatio));
}

void EditionWindow::save()
{
    QImage finalImage = _image_label->pixmap()->toImage();
    QFile initiaImageFile(_image_path);

    if(!initiaImageFile.remove()){
        QMessageBox::information(this, "Erreur", "Erreur lors de la sauvegarde l'image");
        return;
    }

    if(!finalImage.save(_image_path)){
        QMessageBox::information(this, "Erreur", "Erreur lors de la sauvegarde l'image");
    }
}

void EditionWindow::save_as()
{
    QString filename = QFileDialog::getSaveFileName(this);
    qDebug() << "FileName: " << filename;

    QImage finalImage = _image_label->pixmap()->toImage();

    QStringList list = _image_path.split(".");
    QString finalName = filename;
    finalName.append('.');
    finalName.append(list.last());

    if(!finalImage.save(finalName)){
        QMessageBox::information(this, "Erreur", "Erreur lors de la sauvegarde l'image");
    }
}

