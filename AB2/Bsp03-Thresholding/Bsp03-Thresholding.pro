
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bsp03-Thresholding
TEMPLATE = app

win32 {
    INCLUDEPATH += C:\opencv\release\include
    LIBS += -LC:\opencv\release\lib
    LIBS += -lopencv_core330 -lopencv_imgproc330 -lopencv_highgui330 -lopencv_imgcodecs330
}
unix:!macx {
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib
    LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs
}
macx {
    INCLUDEPATH += /usr/local/include
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
    LIBS += -L/usr/local/lib
    LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
