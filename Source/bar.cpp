#include "bar.h"

namespace md {

    bar::bar(){

        m_beats_per_bar = 4;
        m_steps_per_beat = 4;
        m_events_map.clear();
        // not sure..
        //uint8_t meta = msg_t::Meta;
        //uint8_t time_sig = meta_t::TimeSignature;
        //uint8_t size = 4;
        //m_events_map[0].emplace_back({meta,time_sig,size,m_steps_per_beat,(uint8_t)std::sqrt(m_beats_per_bar),})
    }

    bar::bar(uint8_t steps_per_beat, uint8_t beats_per_bar){
        m_steps_per_beat = steps_per_beat;
        m_beats_per_bar = beats_per_bar;
    }

    bar::~bar()= default;

    bar::bar(const bar& other){
        m_beats_per_bar = other.m_beats_per_bar;
        m_steps_per_beat = other.m_steps_per_beat;
        m_events_map = other.m_events_map;

    }

    bar& bar::operator=(const bar& other) = default;

    bar::bar(bar&& other) noexcept{
        m_beats_per_bar = other.m_beats_per_bar;
        m_steps_per_beat = other.m_steps_per_beat;
        m_events_map = std::move(other.m_events_map);
    }

    bar& bar::operator=(bar&& other) noexcept{
        m_beats_per_bar = other.m_beats_per_bar;
        m_steps_per_beat = other.m_steps_per_beat;
        m_events_map = std::move(other.m_events_map);
        return *this;
    }

    void bar::set_time_signature(
            uint8_t steps_per_beat,
            uint8_t beats_per_bar
    ) {
        m_beats_per_bar = beats_per_bar;
        m_steps_per_beat = steps_per_beat;
    }


    bar::events& bar::operator[](size_t pos) {
        return m_events_map[pos];
    }


    const uint32_t bar::beats_per_bar() const {
        return m_beats_per_bar;
    }

    const uint32_t bar::steps_per_beat() const {
        return m_steps_per_beat;
    }

    std::map<size_t, bar::events> &bar::get_events_map() {
        return m_events_map;
    }

    const std::map<size_t, bar::events> &bar::get_events_map() const {
        return m_events_map;
    }

    uint32_t bar::get_step_len(uint32_t time_div) const {
        //return (time_div * 4) / m_beats_per_bar;
        return time_div;
    }
}