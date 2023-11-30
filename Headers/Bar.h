#ifndef MIDI_API_BAR_H
#define MIDI_API_BAR_H

#include "Event.h"
#include "NoteInfo.h"


namespace md {

    struct Bar {
    public:
        Bar() = default;
        Bar(
                std::vector<Event>&& events,
                std::vector<std::vector<NoteInfo>>&& rhythm
        );

        bool m_muted = false;
        std::vector<Event> m_events;
        std::vector<std::vector<NoteInfo>> m_rhythm;
    };

}

#endif //MIDI_API_BAR_H
