cmake_minimum_required(VERSION 3.19)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(hootcurses STATIC ${MY_SOURCES})
target_include_directories(hootcurses PUBLIC include)
