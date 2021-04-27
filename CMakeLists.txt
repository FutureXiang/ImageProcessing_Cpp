cmake_minimum_required(VERSION 3.16)
project(Image)

set(CMAKE_CXX_STANDARD 14)

add_executable(Image main.cpp ByteReader.cpp Image.cpp)

if (APPLE)
    target_link_libraries(Image "-framework OpenGL")
    target_link_libraries(Image "-framework GLUT")
endif ()
