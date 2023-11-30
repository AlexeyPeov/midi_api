#include "../Headers/NoteInfo.h"

namespace md {
    NoteInfo::NoteInfo(uint8_t note, uint32_t start, uint32_t finish) {
        m_note = note;
        m_start = start;
        m_finish = finish;
    }

    bool NoteInfo::is_muted() const {
        return m_note == 0xFF;
    }
} // mapi