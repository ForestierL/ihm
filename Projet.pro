QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addtoalbumwindow.cpp \
    albumbutton.cpp \
    database.cpp \
    editionwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    filepropertieswindow.cpp \
    pathvisit.cpp \
    qhoversensitivebutton.cpp \
    checkingwindow.cpp \
    albumline.cpp

HEADERS += \
    addtoalbumwindow.h \
    albumbutton.h \
    database.h \
    editionwindow.h \
    mainwindow.h \
    filepropertieswindow.h \
    pathvisit.h \
    qhoversensitivebutton.h \
    checkingwindow.h \
    albumline.h

FORMS += \
    addtoalbumwindow.ui \
    editionwindow.ui \
    mainwindow.ui \
    filepropertieswindow.ui \
    checkingwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    arrow-d-w.png \
    arrow-l-b.png \
    arrow-l-w.png \
    arrow-r-b.png \
    arrow-r-w.png \
    arrow-u-b.png \
    arrow-u-w.png \
    crop-w.png \
    delete-b.png \
    delete-r.png \
    delete-w.png \
    home-b.png \
    home-w.png \
    icon-b.png \
    icon-w.png \
    list-b.png \
    list-w.png \
    next-w.png \
    previous-w.png \
    resize-w.png \
    rotate-w.png \
    save-w.png \
    star-empty-w.png \
    star-full-w.png \
    star-half-w.png

RESOURCES += \
    icons.qrc
