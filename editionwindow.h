#ifndef EDITIONWINDOW_H
#define EDITIONWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QLabel>
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
    void create_contents(void);
    void set_image(const QString &fileName);


private: //données membres
    Ui::EditionWindow *ui;

    int _window_width;
    int _window_heigth;

    QString _image_path;
    QLabel* _image_label;
    QImage _initial_image;

    QSlider* _zoom_slider;

private: //méthodes
    QFrame* create_status_bar(void);
    QFrame* create_tool_bar(void);

    void init_background(void);
    void init_connects(void);





protected slots:
    void resize_image(int);
    void save();
    void save_as();


}; //class EditionWindow

#endif // EDITIONWINDOW_H
