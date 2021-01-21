QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    commands.cpp \
    finddialog.cpp \
    global.cpp \
    helpdialog.cpp \
    inputdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    minimapwidget.cpp \
    myarrow.cpp \
    mygraphicsscene.cpp \
    myitem.cpp \
    resizer.cpp

HEADERS += \
    commands.h \
    finddialog.h \
    global.h \
    helpdialog.h \
    inputdialog.h \
    mainwindow.h \
    minimapwidget.h \
    myarrow.h \
    mygraphicsscene.h \
    myitem.h \
    resizer.h

FORMS += \
    finddialog.ui \
    helpdialog.ui \
    inputdialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
