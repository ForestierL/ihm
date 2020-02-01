#include "mainwindow.h"
#include"editionwindow.h"

#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow w;
    w.setStatusBar();
    w.show();
    /*
    EditionWindow w;
            w.setImage("/amuhome/f18013416/IHM/Projet/ihm/chat.jpg");
            w.createContents();
            w.show();
*/
    return a.exec();
}
