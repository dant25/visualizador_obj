#!/bin/bash

qmake -project -r DESTDIR=bin OBJECTS_DIR=obj UI_DIR=temp RCC_DIR=temp MOC_DIR=temp QMAKE_CXXFLAGS+=-std=c++0x QT+=opengl LIBS+=-lGLU
qmake *.pro
