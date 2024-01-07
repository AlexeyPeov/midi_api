#include "bar.h"

namespace md {

    bar::bar(){

        m_beats_per_bar = 4;
        m_quarter_notes_per_beat = 4;
        m_events_map.clear();
    }

    bar::~bar()= default;

    bar::bar(const bar& other){
        m_beats_per_bar = other.m_beats_per_bar;
        m_quarter_notes_per_beat = other.m_quarter_notes_per_beat;
        m_events_map = other.m_events_map;

    }

    bar& bar::operator=(const bar& other) = default;

    bar::bar(bar&& other) noexcept{
        m_beats_per_bar = other.m_beats_per_bar;
        m_quarter_notes_per_beat = other.m_quarter_notes_per_beat;
        m_events_map = std::move(other.m_events_map);
    }

    bar& bar::operator=(bar&& other) noexcept{
        m_beats_per_bar = other.m_beats_per_bar;
        m_quarter_notes_per_beat = other.m_quarter_notes_per_beat;
        m_events_map = std::move(other.m_events_map);
        return *this;
    }

    void bar::set_time_signature(
            uint32_t quarter_notes_per_beat,
            uint32_t beats_per_bar
    ) {
        m_beats_per_bar = beats_per_bar;
        m_quarter_notes_per_beat = quarter_notes_per_beat;
        m_events_map.clear();
    }


    bar::events& bar::operator[](size_t pos) {
        return m_events_map[pos];
    }


    const uint32_t bar::beats_per_bar() const {
        return m_beats_per_bar;
    }

    const uint32_t bar::qn_per_beat() const {
        return m_quarter_notes_per_beat;
    }

    std::map<size_t, bar::events> &bar::get_events_map() {
        return m_events_map;
    }

    const std::map<size_t, bar::events> &bar::get_events_map() const {
        return m_events_map;
    }
}