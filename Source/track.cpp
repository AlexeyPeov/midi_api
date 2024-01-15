#include "track.h"

namespace md {

    track::track(uint32_t time_div){
        m_time_div = time_div;
    };

    void track::add_bar(const bar &bar) {

        std::vector<event> events_vec;
        events_vec.reserve(bar.get_events_map().size());

        uint32_t track_bar_len = m_time_div * bar.steps_per_beat() * (bar.beats_per_bar() / 4);

        uint32_t last_event_time = 0;
        uint32_t curr_event_time = 0;

        auto& events_map = bar.get_events_map();

        for(auto& [time, events] : events_map){
            for(auto& event : events){
                last_event_time = curr_event_time;
                curr_event_time = time;
                auto dt = curr_event_time - last_event_time;
                events_vec.emplace_back(dt, event);
            }
        }
        if(((int)track_bar_len - (int)curr_event_time) < 0) {
            std::cout << "wrong bar length at track::add_bar, "
                         "should be less than or equal to step_len * steps_amount\n"
                         "your length: " << curr_event_time << " max: " << track_bar_len << '\n';
        }


        if((track_bar_len - curr_event_time) > 0){
            auto end_event = std::vector<uint8_t>{msg_t::NoteOn, 127, 0};
            events_vec.emplace_back(track_bar_len - curr_event_time, std::move(end_event));
        }

        m_events.insert(m_events.end(), events_vec.begin(), events_vec.end());
    }

    void track::add_bars(const std::vector<bar> &bars) {
        for(const md::bar& b : bars){
            add_bar(b);
        }
    }

    std::vector<event> &track::get_events() {
        return m_events;
    }

    const std::vector<event> &track::get_events() const {
        return m_events;
    }

    void track::emplace_back(const event &event) {
        m_events.emplace_back(event);
    }

    void track::clear() {
        m_events.clear();
    }


}
