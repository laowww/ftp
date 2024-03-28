QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
DESTDIR = ./bin

include(./curl/curl.prf)

SOURCES += \
    downloadfile.cpp \
    ftpclient.cpp \
    main.cpp \
    mainwindow.cpp \
    progress.cpp

HEADERS += \
    FtpClient.const.h \
    downloadfile.h \
    ftpclient.h \
    ftpclient.struct.h \
    mainwindow.h \
    progress.h

FORMS += \
    downloadfile.ui \
    mainwindow.ui \
    progress.ui

CONFIG(debug, debug|release){
    BUILD_CONFIG = ./debug
}else{
    BUILD_CONFIG = ./release
}

UI_DIR = ./$${BUILD_CONFIG}/ui
MOC_DIR = ./$${BUILD_CONFIG}/moc
RCC_DIR = ./$${BUILD_CONFIG}/rcc
OBJECTS_DIR = ./$${BUILD_CONFIG}/obj

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
