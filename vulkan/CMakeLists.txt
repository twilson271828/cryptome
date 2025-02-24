cmake_minimum_required(VERSION 3.10)
project(BouncingSpheres)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

include_directories(${Vulkan_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})

add_executable(BouncingSpheres main.cpp)
target_link_libraries(BouncingSpheres Vulkan::Vulkan glfw glm)