#ifndef CREATEALBUMWINDOW_H
#define CREATEALBUMWINDOW_H

#include "mainwindow.h"

#include <QDialog>

namespace Ui {
class CreateAlbumWindow;
}

class CreateAlbumWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CreateAlbumWindow(MainWindow *parent);
    ~CreateAlbumWindow();

protected slots:
    void ok_clicked(void);

private:
    Ui::CreateAlbumWindow *ui;
    MainWindow *parent;
};

#endif // CREATEALBUMWINDOW_H
