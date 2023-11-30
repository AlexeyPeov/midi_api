#ifndef MIDI_API_NOTEINFO_H
#define MIDI_API_NOTEINFO_H

#include <cstdint>

namespace md {

    struct NoteInfo {
        uint8_t m_note;
        uint32_t m_start;
        uint32_t m_finish;

        NoteInfo(uint8_t note, uint32_t start, uint32_t finish);

        [[nodiscard]] bool is_muted() const;
    };

}

#endif //MIDI_API_NOTEINFO_H
