#ifndef EDITIONWINDOW_H
#define EDITIONWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QLabel>


namespace Ui {
class EditionWindow;
}

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



}; //class EditionWindow

#endif // EDITIONWINDOW_H
