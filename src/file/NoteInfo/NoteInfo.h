#ifndef SONGGEN_NOTEINFO_H
#define SONGGEN_NOTEINFO_H

#include <cstdint>
#include "enums/note.h"

struct NoteInfo{
    uint32_t start;
    uint32_t finish;
    md::note note = 0;
    uint8_t loudness = 90;
    
    NoteInfo() = default;

    NoteInfo(uint32_t start, uint32_t finish);

    NoteInfo(md::note note, uint32_t start, uint32_t finish, uint8_t loudness);

    uint32_t getNoteLength() const;
};

#endif
