#ifndef FILEPROPERTIESWINDOW_H
#define FILEPROPERTIESWINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {class FilePropertiesWindow;}
QT_END_NAMESPACE

class FilePropertiesWindow : public QDialog
{
    Q_OBJECT

public:
    FilePropertiesWindow(QWidget *parent = nullptr);
    ~FilePropertiesWindow();
    void createContents(void);

private:
    Ui::FilePropertiesWindow *ui;

protected slots:

};

#endif
