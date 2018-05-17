cmake_minimum_required(VERSION 3.9)
project(media)

set(CMAKE_CXX_STANDARD 11)

set(JNI_INCLUDE "$ENV{JAV_HOME}/include")
include_directories(${JNI_INCLUDE})
include_directories(${JNI_INCLUDE}/darwin)
include_directories(/usr/local/include)
LINK_DIRECTORIES(/usr/local/lib)

set(CMAKE_CXX_STANDARD 11)

add_library(media SHARED src/com_codingdie_util_media_MediaUtil.cpp
        src/MediaUtil.c
        src/cJSON.c)
add_executable(mediatest src/main.c src/cJSON.c)

target_link_libraries(media swresample)
target_link_libraries(media avcodec)
target_link_libraries(media avformat)
target_link_libraries(media avfilter)

target_link_libraries(mediatest swresample)
target_link_libraries(mediatest avcodec)
target_link_libraries(mediatest avformat)
target_link_libraries(mediatest avfilter)
INSTALL(TARGETS media
        LIBRARY DESTINATION lib)