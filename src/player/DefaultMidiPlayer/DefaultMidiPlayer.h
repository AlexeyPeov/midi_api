
#ifndef MIDI_API_DEFAULT_MIDI_OUTPUT_H
#define MIDI_API_DEFAULT_MIDI_OUTPUT_H

#if defined(_WIN32) || defined(_WIN64)
#include "WindowsPlayer/WindowsPlayer.h"
typedef md::WindowsPlayer DefaultMidiPlayer;
#elif defined(__APPLE__)
#include "ApplePlayer.h"
typedef md::ApplePlayer DefaultMidiPlayer;
#elif defined(__linux__)
#include "LinuxPlayer.h"
typedef md::LinuxPlayer DefaultMidiPlayer;
#else
    #error "Unknown platform"
#endif



#endif
