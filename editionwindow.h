#ifndef EDITIONWINDOW_H
#define EDITIONWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QLabel>
#include <QGraphicsScene>

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


private:
    QFrame* createStatusBar(void);
    QFrame* createToolBar(void);
    void initBackground(void);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

    Ui::EditionWindow *ui;
    QString imagePath;
    QLabel *imageLabel;
    QPixmap initialPixMap;
    QImage dstImage;
    QImage newImage;
    QRect rect;
    float initialImageWidth;
    float initialImageHeigth;
    float actualImageWidth;
    float actualImageHeigth;
    QPoint topLeft;
    QPoint bottomRight;
    bool crop = false;

protected slots:
    void resizeImage(int);
    void cropImage();
    void verticalMirror();
    void horizontalMirror();
    void rotateImage();

}; //class EditionWindow

#endif // EDITIONWINDOW_H
