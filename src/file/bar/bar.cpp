#include "bar.h"

namespace md
{

bar::bar()
{

    m_beatsPerBar = 4;
    m_stepsPerBeat = 4;
    m_eventsMap.clear();
    // not sure..
    // uint8_t meta = msg_t::Meta;
    // uint8_t time_sig = meta_t::TimeSignature;
    // uint8_t size = 4;
    // m_eventsMap[0].emplace_back({meta,time_sig,size,m_stepsPerBeat,(uint8_t)std::sqrt(m_beatsPerBar),})
}

bar::bar(uint8_t steps_per_beat, uint8_t beats_per_bar)
{
    m_stepsPerBeat = steps_per_beat;
    m_beatsPerBar = beats_per_bar;
}

bar::~bar() = default;

bar::bar(const bar& other)
{
    m_beatsPerBar = other.m_beatsPerBar;
    m_stepsPerBeat = other.m_stepsPerBeat;
    m_eventsMap = other.m_eventsMap;
}

bar& bar::operator=(const bar& other) = default;

bar::bar(bar&& other) noexcept
{
    m_beatsPerBar = other.m_beatsPerBar;
    m_stepsPerBeat = other.m_stepsPerBeat;
    m_eventsMap = std::move(other.m_eventsMap);
}

bar& bar::operator=(bar&& other) noexcept
{
    m_beatsPerBar = other.m_beatsPerBar;
    m_stepsPerBeat = other.m_stepsPerBeat;
    m_eventsMap = std::move(other.m_eventsMap);
    return *this;
}

void bar::setTimeSig(uint8_t steps_per_beat, uint8_t beats_per_bar)
{
    m_beatsPerBar = beats_per_bar;
    m_stepsPerBeat = steps_per_beat;
}

bar::events& bar::operator[](size_t pos)
{
    return m_eventsMap[pos];
}

uint32_t bar::beatsPerBar() const
{
    return m_beatsPerBar;
}

uint32_t bar::stepsPerBeat() const
{
    return m_stepsPerBeat;
}

std::map<size_t, bar::events>& bar::getEventsMap()
{
    return m_eventsMap;
}

const std::map<size_t, bar::events>& bar::getEventsMap() const
{
    return m_eventsMap;
}

uint32_t bar::getStepLen(uint32_t time_div) const
{
    // return (time_div * 4) / m_beatsPerBar;
    return time_div;
}
}