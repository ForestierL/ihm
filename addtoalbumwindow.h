#ifndef ADDTOALBUMWINDOW_H
#define ADDTOALBUMWINDOW_H

#include <QDialog>

namespace Ui {
class AddToAlbumWindow;
}

class AddToAlbumWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddToAlbumWindow(QWidget *parent = nullptr);
    ~AddToAlbumWindow();

private:
    Ui::AddToAlbumWindow *ui;
};

#endif // ADDTOALBUMWINDOW_H
