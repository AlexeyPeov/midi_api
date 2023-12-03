cmake_minimum_required(VERSION 3.25)
project(midi_api)

set(CMAKE_CXX_STANDARD 17)

include_directories(Headers Source)

file(GLOB_RECURSE SOURCES "Headers/*.h" "Source/*.cpp")

add_executable(midi_api main.cpp ${SOURCES})


if(WIN32 OR WIN64)
    target_link_libraries(midi_api winmm)
endif()
