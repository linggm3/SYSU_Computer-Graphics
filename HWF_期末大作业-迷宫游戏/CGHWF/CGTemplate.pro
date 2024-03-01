QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console qt c++11

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += "D:\OpenGL\glew-2.1.0\include"
INCLUDEPATH += "D:\OpenGL\gludlls37beta"


LIBS += \
	Glu32.lib \
	OpenGL32.lib

SOURCES += \
    camera.cpp \
    main.cpp \
    maze.cpp \
    myglwidget.cpp \
    player.cpp \
    textures.cpp \
    vec3.cpp

HEADERS += \
    camera.h \
    maze.h \
    myglwidget.h \
    player.h \
    textures.h \
    vec3.h

DISTFILES += \
    blue_bk.bmp \
    blue_dn.bmp \
    blue_ft.bmp \
    blue_lf.bmp \
    blue_rt.bmp \
    blue_up.bmp \
    wall1.bmp \
    wall2.bmp \
    wall3.bmp
