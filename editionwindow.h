#ifndef EDITIONWINDOW_H
#define EDITIONWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QLabel>
#include <QGraphicsScene>
#include <QSlider>

QT_BEGIN_NAMESPACE
namespace Ui {class EditionWindow;}
QT_END_NAMESPACE


class EditionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditionWindow(QWidget *parent = nullptr);
    ~EditionWindow();
    void createContents(void);
    void setImage(const QString &fileName);
    QImage dstImage;
    QImage newImage;

private:
    QFrame* createStatusBar(void);
    QFrame* createToolBar(void);
    void initBackground(void);

    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);


    void initConnects(void);

    Ui::EditionWindow *ui;
    QString imagePath;
    QLabel *imageLabel;
    QSlider *zoomSlider;
    int windowWidth;
    int windowHeigth;

    bool crop = false;
    float actualImageWidth;
    float actualImageHeigth;
    QRect rect;

protected slots:
    void resizeImage(int);

    void cropImage();
    void verticalMirror();
    void horizontalMirror();
    void rotateImage();
    void resize();

    void save(void);
    void saveAs(void);

}; //class EditionWindow

#endif // EDITIONWINDOW_H
