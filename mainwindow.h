#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>

#include "editionwindow.h"
#include "checkingwindow.h"
#include "ui_mainwindow.h"

#include "filepropertieswindow.h"

#include "pathvisit.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setStatusBar();
    bool updateCurrentPath(QString);
    void setNavButtons();
    void displayAlbum();
    void checkAllPath();

private slots:
    void on_elementListView_doubleClicked(const QModelIndex &index);
    void on_dirTreeView_clicked(const QModelIndex &index);
    void openEditor();
    void openDirectory();
    void addToAlbum();
    void informations();
    void eraseItem();
    bool removeDirectory(QString dirPath = "");
    void showContextMenu(const QPoint&);
    void on_lePath_returnPressed();
    void home_clicked();
    void previous_clicked();
    void next_clicked();
    void up_clicked();
    void createNewButtonAlbum(QString name);
    void create_album(const QString&);

    void recent_folder();
    void recent_album();
    void new_album();
    void add_to_album();
    void close();
    void edit();
    void rename();
    void copy();
    void cut();
    void paste();
    void small_icons();
    void medium_icons();
    void big_icons();
    void list();
    void icons();
    void dark_theme();
    void light_theme();
    void about();
    void manual();

    void on_pbAddAlbum_clicked();
    void generateCreateAlbumLine();

    void delete_album(const QString&);
    void open_album(const QString&);

private:
    Ui::MainWindow *ui;
    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;
    QString actualFile;
    PathVisit *pathVisit;
    bool newAlbum = false;

    void createActions();
};
#endif // MAINWINDOW_H
