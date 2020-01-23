#include "mainwindow.h"
#include"editionwindow.h"

#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EditionWindow w;
    w.setImage("/Users/romaincolonnadistria/Desktop/ihm/chat.jpg");
    w.createContents();
    w.show();

    return a.exec();
}
