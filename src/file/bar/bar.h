#ifndef MIDI_API_BAR_H
#define MIDI_API_BAR_H

#include <cmath>
#include <cstdint>
#include <map>
#include <stdexcept>
#include <vector>

#include "event/event.h"

namespace md
{

class bar
{
public:
    explicit bar();

    bar(uint8_t stepsPerBeat, uint8_t beatsPerBar);

    ~bar();

    bar(const bar& other);

    bar& operator=(const bar& other);

    bar(bar&& other) noexcept;

    bar& operator=(bar&& other) noexcept;

    void setTimeSig(uint8_t stepsPerBeat, uint8_t beatsPerBar);

    uint32_t getStepLen(uint32_t timeDiv) const;

    // a vector of events at position
    // event is a vector of uint8_t
    class events : public std::vector<std::vector<uint8_t>>
    {
    public:
        events() = default;

        events& operator+=(const std::vector<uint8_t>& event)
        {
            emplace_back(event);
            return *this;
        }

        events& operator+=(std::initializer_list<uint8_t> event)
        {
            emplace_back(event);
            return *this;
        }
    };

    bar::events& operator[](size_t pos);

    uint32_t beatsPerBar() const;

    uint32_t stepsPerBeat() const;

    std::map<size_t, events>& getEventsMap();

    const std::map<size_t, events>& getEventsMap() const;

private:
    uint8_t m_beatsPerBar;
    uint8_t m_stepsPerBeat;
    std::map<size_t, events> m_eventsMap;
};

}

#endif
