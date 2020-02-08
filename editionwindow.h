#ifndef EDITIONWINDOW_H
#define EDITIONWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QLabel>

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

    Ui::EditionWindow *ui;
    QString imagePath;
    QLabel *imageLabel;

    QPixmap initialPixMap;
    float initialImageWidth;
    float initialImageHeigth;

protected slots:
    void resizeImage(int);
    void cropImage();


}; //class EditionWindow

#endif // EDITIONWINDOW_H
