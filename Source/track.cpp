#include "track.h"

namespace md {

    track::track(uint32_t qnl){
        m_qnl = qnl;
    };

    void track::add_bar(const bar &bar) {

        auto bar_len = bar::get_qn_sz() * bar.qn_per_beat() * bar.beats_per_bar();
        auto bar_qnl = bar::get_qn_sz();
        size_t beats_amount = bar.size();

        auto track_bar_len = m_qnl * bar.qn_per_beat() * bar.beats_per_bar();

        uint32_t last_event_time = 0;
        uint32_t curr_event_time = 0;

        uint32_t beat_len = bar_qnl * bar.qn_per_beat();


        for (size_t i = 0; i < beats_amount; ++i) {

            size_t qn_amount = bar[i].size();

            for (size_t j = 0; j < qn_amount; ++j) {

                auto &event_map = bar[i][j];

                for (auto &[pos, events]: event_map) {

                    double mul = (double)(pos + (i*beat_len) + (j*bar_qnl)) / (double)bar_len;
                    double time = (double)track_bar_len * mul;

                    std::for_each(
                            events.begin(), events.end(),
                            [&](const auto &ev){

                                // todo : possible redundant calculations
                                last_event_time = curr_event_time;
                                curr_event_time = (uint32_t)time;
                                auto dt = curr_event_time - last_event_time;
                                m_events.emplace_back(dt, ev);

                            }
                    );

                }

            }
        }


        if((track_bar_len - curr_event_time) != 0){
            auto end_event = std::vector<uint8_t>{(uint8_t)MidiMessageType::NoteOn, 127, 0};
            m_events.emplace_back(track_bar_len - curr_event_time, std::move(end_event));
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
