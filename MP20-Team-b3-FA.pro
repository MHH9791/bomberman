QT       += core gui
CONFIG += qt
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controller.cpp \
    gamemodel.cpp \
    main.cpp \
    mainwindow.cpp\
    pathfinder.cpp\
    world.cpp \
    xenemy.cpp \
    zenemy.cpp

HEADERS += \
    controller.h \
    gamemodel.h \
    graphicComponent.h \
    mainwindow.h\
    template.h \
    pathfinder.h\
    textComponent.h \
    world.h \
    xenemy.h \
    zenemy.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc


#macx: LIBS += -L$$PWD/../lib/ -lworld.1.0.0

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
