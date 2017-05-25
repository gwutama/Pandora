cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(Pandora)

set(CMAKE_PREFIX_PATH /opt/Qt/5.7/clang_64/lib/cmake)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)

add_definitions(-Wfatal-errors) # stop compilation on first error

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)
