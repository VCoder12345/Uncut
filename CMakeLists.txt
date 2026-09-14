cmake_minimum_required(VERSION 3.22)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_PREFIX_PATH "D:/Programme/QT/newinstall/6.9.1/msvc2022_64;D:/Coding/c++/packages/SDL3-3.2.10/")
set(SDL3_INCLUDE_DIRS "D:/Coding/c++/packages/SDL3-3.2.10/include/")
set(SDL3_DLL_PATH "D:/Coding/c++/packages/SDL3-3.2.10/lib/x64/SDL3.dll")

project(Uncut VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
find_package(FFMPEG REQUIRED)

qt_standard_project_setup()

add_subdirectory(src)


