#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>

#include "editionwindow.h"
#include "checkingwindow.h"
#include "ui_mainwindow.h"
#include "filepropertieswindow.h"

#include "pathvisit.h"
#include "itemlist.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString theme;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setStatusBar();
    bool updateCurrentPath(QString);
    void setNavButtons();
    void displayAlbum();
    void checkAllPath();
    void getChildAndSetStyle(QObject *, QString);
    void setTheme();
    Ui::MainWindow* getUI(void);
    void reloadImageItem();

public slots:
    void openEditor(const QString path = "");

private slots:
    void on_elementListView_doubleClicked(const QModelIndex &index);
    void on_dirTreeView_clicked(const QModelIndex &index);
    void openDirectory();
    void addToAlbum(const QString path = "");
    void informations(const QString path = "");
    void eraseItem(const QString path = "");
    bool removeDirectory(QString dirPath = "");
    void showContextMenu(const QPoint&);
    void on_lePath_returnPressed();
    void home_clicked();
    void previous_clicked();
    void next_clicked();
    void up_clicked();
    void createNewButtonAlbum(QString name);
    void create_album(const QString&);
    void getImageFromFilter(const QString &color, const QString &feeling, const QString &score);

    void recent_folder();
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

    void allImage_clicked();

private:
    Ui::MainWindow *ui;
    ItemList *itemList;
    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;
    QString actualFile;
    PathVisit *pathVisit;
    QLabel *statusMessage;
    bool newAlbum = false;

    bool isFilterFormDisplayed = false;
    QFrame *currentFilterForm;

    int currentDisplayedAlbumId;

    void createActions();
    void addRecentsAlbumToMenuFichier(void);
    void deleteFilterForm(void);

};
#endif // MAINWINDOW_H
