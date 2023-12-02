#include "Bar.h"

namespace md {
    Bar::Bar(
            std::vector<Event> &&events,
            std::vector<std::vector<NoteInfo>> &&rhythm
    )
    {
        m_events = std::move(events);
        m_rhythm = std::move(rhythm);
    }
} // mapi