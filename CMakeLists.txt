cmake_minimum_required(VERSION 3.16)
project(Image)

set(CMAKE_CXX_STANDARD 14)

if (WIN32)
    include_directories(include)
    link_directories(lib/x64)
endif()

add_executable(Image main.cpp ByteReader.cpp Image.cpp Color.cpp Mathematics.cpp)

if (APPLE)
    target_link_libraries(Image "-framework OpenGL")
    target_link_libraries(Image "-framework GLUT")
elseif(WIN32)
    target_link_libraries(Image libopengl32.a libfreeglut.a freeglut.dll)
endif ()
