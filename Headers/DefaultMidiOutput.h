
#ifndef MIDI_API_DEFAULT_MIDI_OUTPUT_H
#define MIDI_API_DEFAULT_MIDI_OUTPUT_H

#if defined(_WIN32) || defined(_WIN64)
#include "WindowsPlayer.h"
typedef md::WindowsPlayer DefaultMidiOutput;
#elif defined(__APPLE__)
#include "ApplePlayer.h"
typedef md::ApplePlayer DefaultMidiOutput;
#elif defined(__linux__)
#include "LinuxPlayer.h"
typedef md::LinuxPlayer DefaultMidiOutput;
#else
    #error "Unknown platform"
#endif



#endif
