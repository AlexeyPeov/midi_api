#pragma once

#include <cstdint>

namespace md
{
enum meta_t : uint8_t
{
    SequenceNumber = 0x00, // size 2
    Text = 0x01,
    Copyright = 0x02,
    TrackName = 0x03,
    InstrumentName = 0x04,
    Lyrics = 0x05,
    Marker = 0x06,
    CuePoint = 0x07,
    ChannelPrefix = 0x20, // size 1
    OutputCable = 0x21, // size 1
    EndOfTrack = 0x2f, // size 0
    Tempo = 0x51, // size 3
    SmpteOffset = 0x54, // size 5
    TimeSignature = 0x58,
    KeySignature = 0x59,
    NO = 0x66
};

}