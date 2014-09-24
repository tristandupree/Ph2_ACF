#-------------------------------------------------
#
# Project created by QtCreator 2014-09-10T13:51:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUI
TEMPLATE = app

CONFIG += c++11 debug

QMAKE_CXXFLAGS += -g -O1 -w -pedantic -fPIC -std=c++11 `root-config --cflags --evelibs` -Wcpp

LIBS += -L/usr/include/boost -lboost_system -lboost_thread -lpthread -lboost_regex
LIBS += -L../lib -lPh2_Interface -lPh2_Description -lPh2_System -lPh2_Tools
LIBS += -L/opt/cactus/lib -lcactus_extern_pugixml -lcactus_uhal_log -lcactus_uhal_grammars -lcactus_uhal_uhal
LibraryDirs = /opt/cactus/lib /opt/xdaq/lib ../
LibraryPaths = $(LibraryDirs:%=-L%)
LIBS += $(LibraryPaths:%=-L%) -uhal `root-config --glibs`

LIBS += -L/usr/lib/ -lqjson
INCLUDEPATH += /usr/include/qjson/

INCLUDEPATH += /opt/cactus/include /home/xtaldaq/Ph2DAQ_dev/



SOURCES += main.cpp\
        View/mainview.cpp \
    startup.cpp \
    View/setuptab.cpp \
    ViewMgr/setuptabviewmanager.cpp \
    Model/settings.cpp \
    Model/systemcontroller.cpp \
    View/cbcregisterstab.cpp \
    ViewMgr/cbcregviewmanager.cpp \
    provider.cpp \
    lib/qcustomplot.cpp \
    View/datatesttab.cpp \
    Model/datatest.cpp \
    ViewMgr/datatestviewmanager.cpp

HEADERS  += View/mainview.h \
    startup.h \
    View/setuptab.h \
    ViewMgr/setuptabviewmanager.h \
    Model/settings.h \
    utils.h \
    Model/systemcontroller.h \
    View/cbcregisterstab.h \
    ViewMgr/cbcregviewmanager.h \
    provider.h \
    lib/qcustomplot.h \
    View/datatesttab.h \
    Model/datatest.h \
    ViewMgr/datatestviewmanager.h

FORMS    += View/mainview.ui \
    View/setuptab.ui \
    View/cbcregisterstab.ui \
    View/datatesttab.ui

OTHER_FILES +=

RESOURCES += \
    Resources.qrc
