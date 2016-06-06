#--- qmake parameter start ---
# PLATFORM=WINDOWS
#   or
# PLATFORM=QNX
#   or
# PLATFORM=LINUX
#--- qmake parameter end ---

TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

equals(PLATFORM, WINDOWS) {
    DEFINES += _WINDOWS_PLAT_   #surport:_WINDOWS_PLAT_, _QNX_PLAT_, _LINUX_PLAT_
    LIBS += -lwsock32
}equals(PLATFORM, QNX) {
    DEFINES += _QNX_PLAT_   #surport:_WINDOWS_PLAT_, _QNX_PLAT_, _LINUX_PLAT_
    LIBS += -lsocket
}equals(PLATFORM, LINUX) {
    DEFINES += _LINUX_PLAT_   #surport:_WINDOWS_PLAT_, _QNX_PLAT_, _LINUX_PLAT_
    LIBS += -lrt -lpthread
    QMAKE_CXXFLAGS += -std=c++11
}

LIBS +=-L../bin -ltriceps -lclips -liconv

SOURCES += \
    CepMain.cpp

include(../brms/brms.pri)
include(deployment.pri)
qtcAddDeployment()

OTHER_FILES +=
