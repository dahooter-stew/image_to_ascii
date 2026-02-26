cmake_minimum_required(VERSION 3.19)

project(img_ascii)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(libs/hootcurses)
add_subdirectory(libs/stb_image)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
add_executable(${CMAKE_PROJECT_NAME} ${MY_SOURCES})
#add_executable(${CMAKE_PROJECT_NAME} ${MY_SOURCES} bt.rc) #with icon

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE hootcurses stb_image)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
