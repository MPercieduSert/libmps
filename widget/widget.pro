#-------------------------------------------------
#
# Project created by QtCreator 2019-08-23T19:24:26
#
#-------------------------------------------------

QT       += sql widgets

TARGET = widget
TEMPLATE = lib
CONFIG += staticlib c++17 create_prl

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BarPlotter.cpp \
    EditTreeWidget.cpp \
    Histogramme.cpp \
    AbstractTableModel.cpp \
    MotCleDelegate.cpp \
    MotCleModel.cpp \
    MotCleWidget.cpp \
    NewMotCleDialog.cpp \
    Plotter.cpp \
    ReadTreeWidget.cpp \
    SelectDonneeDialog.cpp \
    SelectInListBox.cpp \
    SpinBoxAnnee.cpp \
    SpinBoxDecimale.cpp \
    SpinBoxNumExclu.cpp \
    TreeModelEditEntity.cpp \
    TreeModelMotCle.cpp \
    TreeModelReadEntity.cpp \
    TreeModelReadTemp.cpp \
    TreeModelTemp.cpp \
    AbstractTreeModel.cpp \
    NewModifDialog.cpp

HEADERS += \
    BarPlotter.h \
    EditTreeWidget.h \
    Histogramme.h \
    AbstractTableModel.h \
    MotCleDelegate.h \
    MotCleModel.h \
    MotCleWidget.h \
    NewMotCleDialog.h \
    Plotter.h \
    ReadTreeWidget.h \
    SelectDonneeDialog.h \
    SelectInListBox.h \
    SpinBoxAnnee.h \
    SpinBoxDecimale.h \
    SpinBoxNumExclu.h \
    TreeModelEditEntity.h \
    TreeModelMotCle.h \
    TreeModelReadEntity.h \
    TreeModelReadTemp.h \
    TreeModelTemp.h \
    AbstractTreeModel.h \
    NewModifDialog.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

SUBDIRS += \
    widget.pro

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../entities/release/ -lentities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../entities/debug/ -lentities
else:unix: LIBS += -L$$OUT_PWD/../entities/ -lentities

INCLUDEPATH += $$PWD/../entities
DEPENDPATH += $$PWD/../entities

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../entities/release/libentities.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../entities/debug/libentities.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../entities/release/entities.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../entities/debug/entities.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../entities/libentities.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../manager/release/ -lmanager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../manager/debug/ -lmanager
else:unix: LIBS += -L$$OUT_PWD/../manager/ -lmanager

INCLUDEPATH += $$PWD/../manager
DEPENDPATH += $$PWD/../manager

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../manager/release/libmanager.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../manager/debug/libmanager.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../manager/release/manager.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../manager/debug/manager.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../manager/libmanager.a
