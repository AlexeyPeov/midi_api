cmake_minimum_required(VERSION 3.22)
project(midi_api)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wpedantic")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG")

function(include_all_subdirectories base_dir)
    file(GLOB_RECURSE subdirs LIST_DIRECTORIES true ${base_dir}/*)
    foreach(subdir ${subdirs})
        if(IS_DIRECTORY ${subdir})
            include_directories(${subdir})
        endif()
    endforeach()
endfunction()

include_directories(src)
include_all_subdirectories(${CMAKE_SOURCE_DIR}/src/)


file(GLOB_RECURSE SOURCES "src/*.h" "src/*.cpp")

add_executable(midi_api main.cpp ${SOURCES})

target_precompile_headers(midi_api PRIVATE "${CMAKE_SOURCE_DIR}/src/pch/pch.h")

if(WIN32 OR WIN64)
    target_link_libraries(midi_api winmm)
endif()
