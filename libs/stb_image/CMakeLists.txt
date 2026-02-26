cmake_minimum_required(VERSION 3.19)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(stb_image STATIC ${MY_SOURCES})
target_include_directories(stb_image PUBLIC include)

target_link_libraries(stb_image PRIVATE m)
