QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    allpath.cpp \
    diygame.cpp \
    diymode.cpp \
    main.cpp \
    mainwindow.cpp \
    randommode.cpp \
    selectGame.cpp \
    selectmode.cpp \

HEADERS += \
    allpath.h \
    diygame.h \
    diymode.h \
    mainwindow.h \
    randommode.h \
    selectGame.h \
    selectmode.h \

FORMS += \
    allpath.ui \
    diygame.ui \
    diymode.ui \
    mainwindow.ui \
    randommode.ui \
    selectGame.ui \
    selectmode.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
