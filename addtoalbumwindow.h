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
    explicit AddToAlbumWindow(QWidget *parent, int imageId);
    ~AddToAlbumWindow();

private:
    Ui::AddToAlbumWindow *ui;
    int _currentImageid;

protected slots:
    void ok_clicked(void);

};

#endif // ADDTOALBUMWINDOW_H
