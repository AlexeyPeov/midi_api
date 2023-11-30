cmake_minimum_required(VERSION 3.25)
project(midi_api)

set(CMAKE_CXX_STANDARD 17)

add_executable(midi_api
        main.cpp
        Headers/constants.h
        Headers/Bar.h Source/Bar.cpp
        Headers/Event.h Source/Event.cpp
        Headers/File.h Source/File.cpp
        Headers/IOHelper.h Source/IOHelper.cpp
        Headers/NoteInfo.h Source/NoteInfo.cpp
        Headers/Track.h Source/Track.cpp
        )
