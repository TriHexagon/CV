TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp


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
