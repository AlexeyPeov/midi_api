#include "track.h"

namespace md
{

track::track(uint32_t time_div)
{
    m_time_div = time_div;
};

void track::addBar(const bar& bar)
{

    std::vector<event> events_vec;
    auto& events_map = bar.getEventsMap();

    events_vec.reserve(events_map.size());

    uint32_t last_event_time = 0;
    uint32_t curr_event_time = 0;

    for (auto& [time, events] : events_map)
    {
        for (auto& event : events)
        {
            last_event_time = curr_event_time;
            curr_event_time = time;
            auto dt = curr_event_time - last_event_time;
            events_vec.emplace_back(dt, event);
        }
    }

    // uint32_t track_bar_len = m_time_div * bar.steps_per_beat() *
    // (bar.beats_per_bar() / 4);

    // if(((int)track_bar_len - (int)curr_event_time) < 0) {
    //     std::cout << "wrong bar length at track::addBar, "
    //                  "should be less than or equal to step_len *
    //                  steps_amount\n" "your length: " << curr_event_time << "
    //                  max: " << track_bar_len << '\n';
    // }

    // if((track_bar_len - curr_event_time) > 0){
    //     auto end_event = std::vector<uint8_t>{msg_t::NoteOn, 127, 0};
    //     events_vec.emplace_back(track_bar_len - curr_event_time, end_event);
    // }

    m_events.insert(m_events.end(), events_vec.begin(), events_vec.end());
}

std::vector<NoteInfo> track::toNoteInfoVec() const
{
    std::vector<NoteInfo> ret;

    const std::vector<md::event>& events = getEvents();

    std::cout << "EVENTS SIZ: " << events.size() << '\n';

    uint32_t start = 0;

    std::set<uint8_t> usedNotes;

    for (auto it = events.begin(); it != events.end(); ++it)
    {

        const md::event& event = *it;

        start += event.dt();
        // uint8_t msg = event.getMessageVec()[0];

        // if (msg >= md::msg_t::NoteOff && msg <= md::msg_t::NoteOffEnd)
        // {
        //     printf(
        //         "event is note off, e: %#04x, note: %d, vel: %d, isNoteOff: %d "
        //         "\n",
        //         event.getMessageVec()[0],
        //         event.getNote(),
        //         event.getMessageVec()[2],
        //         event.isNoteOff()
        //     );
        // }

        // printf("event is unique note on, e: %#04x, note: %d, vel: %d\n",
        // event.getMessageVec()[0], event.getNote(),
        // event.getMessageVec()[2]);

        if (!event.isNoteOn())
        {
            printf("event is not note on, %#04x\n", event.getMessageVec()[0]);
            std::cout << "event is not note on " << event.getMessageVec()[1]
                      << '\n';
            continue;
        }

        uint8_t note = static_cast<uint8_t>(event.getNote());

        if (usedNotes.contains(note))
        {
            continue;
        }

        usedNotes.emplace(note);

        std::stack<NoteInfo> sameNoteInfos;

        sameNoteInfos.emplace(note, start, 0, 100);

        uint32_t finish = start;

        for (auto nextIt = it + 1; nextIt != events.end(); ++nextIt)
        {

            const md::event& otherEvent = *nextIt;

            finish += otherEvent.dt();

            if (otherEvent.getNote() == event.getNote()
                && otherEvent.isNoteOn())
            {
                sameNoteInfos.emplace(note, finish, 0, 100);
                // std::cout << "stack sz: " << sameNoteInfos.size() << '\n';
            }

            if (otherEvent.isNoteOff())
            {

                if (sameNoteInfos.empty())
                {
                    // std::cout << "bad midi file given at
                    // track::toNoteInfoVec, note off events > note on
                    // events..\n";
                }
                else
                {
                    sameNoteInfos.top().finish = finish;
                    ret.emplace_back(sameNoteInfos.top());
                    sameNoteInfos.pop();
                }
            }
        }
    }

    std::sort(
        ret.begin(),
        ret.end(),
        [](const NoteInfo& a, const NoteInfo& b)
        {
            return a.start < b.start;
        }
    );

    // for(auto& noteInfo : ret){
    //     std::cout << "note: " << (int)noteInfo.note << " start: " <<
    //     noteInfo.start << " finish: " << noteInfo.finish << "\n";
    // }

    return ret;
}

void track::addBars(const std::vector<bar>& bars)
{
    for (const md::bar& b : bars)
    {
        addBar(b);
    }
}

const std::vector<event>& track::getEvents() const
{
    return m_events;
}

void track::emplace_back(const event& event)
{
    m_events.emplace_back(event);
}

void track::clear()
{
    m_events.clear();
}

}