#!/usr/bin/bash

#g++ -fPIC -std=c++17 -o sphere sphere.cpp -I/opt/Qt/6.8.0/gcc_64/include -I/opt/Qt/6.8.0/gcc_64/include/QtOpenGLWidgets -I/opt/Qt/6.8.0/gcc_64/include/QtCore -I/opt/Qt/6.8.0/gcc_64/include/QtGui -I/opt/Qt/6.8.0/gcc_64/include/QtWidgets -I/opt/Qt/6.8.0/gcc_64/include/QtOpenGL -lQt6Core -lQt6Gui -lQt6Widgets -lQt6OpenGL -lGL

g++ -fPIC -std=c++17 -o sphere sphere.cpp -I/opt/Qt/6.8.0/gcc_64/include -I/opt/Qt/6.8.0/gcc_64/include/QtOpenGLWidgets -I/opt/Qt/6.8.0/gcc_64/include/QtCore -I/opt/Qt/6.8.0/gcc_64/include/QtGui -I/opt/Qt/6.8.0/gcc_64/include/QtWidgets -I/opt/Qt/6.8.0/gcc_64/include/QtOpenGL -lQt6Core -lQt6Gui -lQt6Widgets -lQt6OpenGL -lGL -lGLEW
