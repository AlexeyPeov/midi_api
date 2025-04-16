#pragma once

#include <cstdint>

namespace md
{

enum msg_t : uint8_t
{
    UNDEFINED_MSG_T = 0x00,
    // voice
    NoteOff = 0x80,
    NoteOffEnd = 0x89,
    NoteOn = 0x90,
    NoteOnEnd = 0x99,
    NoteAftertouch = 0xa0,
    ControlChange = 0xb0,
    ProgramChange = 0xc0, // size 1
    ChannelAftertouch = 0xd0, // size 1
    PitchWheel = 0xe0,

    // system common
    SysExBegin = 0xf0,
    MtcQuarterFrame = 0xf1,
    SongPositionPointer = 0xf2,
    SongSelect = 0xf3,
    TuneRequest = 0xf6,
    SysExEnd = 0xf7,

    // realtime
    Clock = 0xf8,
    Tick = 0xf9,
    Start = 0xfa,
    Continue = 0xfb,
    Stop = 0xfc,
    ActiveSense = 0xfe,
    Reset = 0xff,

    // Meta events
    Meta = 0xff
};

}