#--- qmake parameter start ---
# PLATFORM=WINDOWS
#   or
# PLATFORM=QNX
#   or
# PLATFORM=LINUX
#--- qmake parameter end ---

TEMPLATE = app

QT += qml quick

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

LIBS += -L../bin -ltriceps -lclips -liconv

TRANSLATIONS += *.ts
CODECFORTR += UTF-8

include(../brms/brms.pri)
SOURCES += main.cpp \
    BrmsAdaptor.cpp

RESOURCES += \
    qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

#TRANSLATIONS=DNDemo_ja_JP.ts

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    BrmsAdaptor.h \
    BrmsInterface.h

OTHER_FILES += \
    setting.pro.user \
    setting.pro.user.bbc2538 \
    RuleChecker.pro.user

RC_FILE = icon.rc
