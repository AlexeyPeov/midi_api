#include "bar.h"

namespace md {

    bar::bar(){

        m_beats_per_bar = 4;
        m_quarter_notes_per_beat = 4;

        m_beats.clear();

        m_beats.resize(m_beats_per_bar);

        for(auto& beat : m_beats){
            beat.resize(m_quarter_notes_per_beat);
        }
    }

    bar::~bar()= default;

    bar::bar(const bar& other){
        m_beats_per_bar = other.m_beats_per_bar;
        m_quarter_notes_per_beat = other.m_quarter_notes_per_beat;
        m_beats = other.m_beats;
    }

    bar& bar::operator=(const bar& other) = default;

    bar::bar(bar&& other) noexcept{
        m_beats_per_bar = other.m_beats_per_bar;
        m_quarter_notes_per_beat = other.m_quarter_notes_per_beat;
        m_beats = std::move(other.m_beats);
    }

    bar& bar::operator=(bar&& other) noexcept{
        m_beats_per_bar = other.m_beats_per_bar;
        m_quarter_notes_per_beat = other.m_quarter_notes_per_beat;
        m_beats = std::move(other.m_beats);
        return *this;
    }

    void bar::set_time_signature(
            uint32_t quarter_notes_per_beat,
            uint32_t beats_per_bar
    ) {
        m_beats_per_bar = beats_per_bar;
        m_quarter_notes_per_beat = quarter_notes_per_beat;

        m_beats.clear();

        m_beats.resize(m_beats_per_bar);
        for(auto& beat : m_beats){
            beat.resize(m_quarter_notes_per_beat);
        }
    }


    bar::beat& bar::operator[](size_t pos) {
        assert(pos < m_beats.size());
        return m_beats[pos];
    }

    const bar::beat & bar::operator[](size_t pos) const {
        assert(pos < m_beats.size());
        return m_beats[pos];
    }

    size_t bar::size() const {
        return m_beats.size();
    }

    uint32_t bar::get_qn_sz() {
        return m_qn_sz;
    }

    const uint32_t bar::beats_per_bar() const {
        return m_beats_per_bar;
    }

    const uint32_t bar::qn_per_beat() const {
        return m_quarter_notes_per_beat;
    }

    bar::events &bar::last_event() {
        return m_beats[m_beats.size()-1][m_quarter_notes_per_beat-1][m_qn_sz];
    }

}