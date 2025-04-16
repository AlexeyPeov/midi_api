#include "NoteInfo.h"

NoteInfo::NoteInfo(uint32_t start, uint32_t finish)
    : start(start), finish(finish)
{
    loudness = 90;
}

NoteInfo::NoteInfo(
    md::note note,
    uint32_t start,
    uint32_t finish,
    uint8_t loudness
)
    : start(start), finish(finish), note(note), loudness(loudness)
{
}

uint32_t NoteInfo::getNoteLength() const
{
    return finish - start;
}
