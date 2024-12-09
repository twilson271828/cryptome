cmake_minimum_required(VERSION 3.10)

# Project name
project(SphereWidget)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find required packages

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# Add the executable
add_executable(SphereWidget sphere.cpp)

# Include directories
target_include_directories(SphereWidget PRIVATE ${Qt6Core_INCLUDE_DIRS} ${Qt6Gui_INCLUDE_DIRS} ${Qt6Widgets_INCLUDE_DIRS} ${Qt6OpenGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

#message(STATUS "Qt6OpenGLWidgets_INCLUDE_DIRS: ${Qt6OpenGLWidgets_INCLUDE_DIRS}")
#message(STATUS "Qt6Widgets_INCLUD_DIRS:  ${Qt6Widgets_INCLUDE_DIRS}")


# Link libraries
target_link_libraries(SphereWidget PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::OpenGL OpenGL::GL GLEW::GLEW)