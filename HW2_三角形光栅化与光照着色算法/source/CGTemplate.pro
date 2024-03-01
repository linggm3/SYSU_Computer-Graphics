QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console qt c++11

DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += "./glm"

INCLUDEPATH += "D:\OpenGL\glew-2.1.0\include"



LIBS += \
	Glu32.lib \
	OpenGL32.lib

SOURCES += \
    main.cpp \
    myglwidget.cpp \
    utils.cpp

HEADERS += \
    myglwidget.h  \
    utils.h