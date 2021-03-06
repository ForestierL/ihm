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
    FilePropertiesWindow(QWidget *parent = nullptr, QString itemPath = "a/b/default");
    ~FilePropertiesWindow();
    void createContents(void);
    void setEditMode(bool);
    void editSwitch();
    bool save();

private:
    Ui::FilePropertiesWindow *ui;
    bool editMode;
    QString itemPath;

protected slots:

private slots:
    void on_ok_clicked();
    void on_edit_clicked();
};

#endif
