QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    outputgetter.cpp \
    pythoninterpreter.cpp

HEADERS += \
    mainwindow.h \
    outputgetter.h \
    pythoninterpreter.h

FORMS += \
    mainwindow.ui
	
INCLUDEPATH += \
    C:\Users\alexa\AppData\Local\Programs\Python\Python39\include

LIBS += \
    C:\Users\alexa\AppData\Local\Programs\Python\Python39\libs\python39.lib
    

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ressources.qrc
