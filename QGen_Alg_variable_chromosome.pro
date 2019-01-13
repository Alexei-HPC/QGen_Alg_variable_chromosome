QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32:INCLUDEPATH += "C:/Program Files (x86)/MPICH2/include/"
unix:INCLUDEPATH += "/usr/include/mpich2/"

SOURCES += \
    main.cpp \
    Bus.cpp \
    Buses.cpp \
    BusStopsOnRoutes.cpp \
    EdgeCost.cpp \
    Event.cpp \
    Events.cpp \
    Individual.cpp \
    MinimalDriveTime.cpp \
    Parameters_.cpp \
    Passanger.cpp \
    PassengersDistribution.cpp \
    Population.cpp \
    Rout.cpp \
    Routes.cpp \
    Stop.cpp \
    tinystr.cpp \
    tinyxml.cpp \
    tinyxmlerror.cpp \
    tinyxmlparser.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Bus.h \
    Buses.h \
    BusStopsOnRoutes.h \
    EdgeCost.h \
    Event.h \
    Events.h \
    Individual.h \
    MinimalDriveTime.h \
    Parameters_.h \
    Passanger.h \
    PassengersDistribution.h \
    Population.h \
    Rout.h \
    Routes.h \
    Stop.h \
    tinystr.h \
    tinyxml.h

#LIBS += -L/"C:/Program Files (x86)/MPICH2/lib" -lmpich2 -lopa -lpthread -lrt

QMAKE_CXXFLAGS += -Bsymbolic-functions

win32:CONFIG(release, debug|release): LIBS += -L'C:/Program Files (x86)/MPICH2/lib/' -lmpich2
else:win32:CONFIG(debug, debug|release): LIBS += -L'C:/Program Files (x86)/MPICH2/lib/' -lmpich2d
else:unix: LIBS += -L'C:/Program Files (x86)/MPICH2/lib/' -lmpich2

INCLUDEPATH += 'C:/Program Files (x86)/MPICH2/include'
DEPENDPATH += 'C:/Program Files (x86)/MPICH2/include'
