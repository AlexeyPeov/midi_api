#include "track.h"

namespace md {

    track::track(uint32_t qnl){
        m_qnl = qnl;
    };

    void track::add_bar(const bar &bar) {

        std::vector<event> events_vec;
        events_vec.reserve(bar.get_events_map().size());

        int track_bar_len = m_qnl * bar.qn_per_beat() * bar.beats_per_bar();

        int last_event_time = 0;
        int curr_event_time = 0;

        auto& events_map = bar.get_events_map();

        for(auto& [time, events] : events_map){

            for(auto& event : events){

                last_event_time = curr_event_time;
                curr_event_time = time;
                auto dt = curr_event_time - last_event_time;
                events_vec.emplace_back(dt, event);

            }
        }
        if((track_bar_len - curr_event_time) < 0) {
            std::cout << "wrong bar length at track::add_bar, "
                         "should be less than or equal to beats_amount * delta_time * quarternotes_amount\n"
                         "your length: " << curr_event_time << " max: " << track_bar_len << '\n';

                         "your length: " << curr_event_time << " max: " << track_bar_len;
            return;
        }


        if((track_bar_len - curr_event_time) > 0){
            auto end_event = std::vector<uint8_t>{msg_t::NoteOn, 127, 0};
            events_vec.emplace_back(track_bar_len - curr_event_time, std::move(end_event));
        }

        m_events.insert(m_events.end(), events_vec.begin(), events_vec.end());
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
