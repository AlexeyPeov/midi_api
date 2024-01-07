#ifndef MIDI_API_BAR_H
#define MIDI_API_BAR_H

#include <cstdint>
#include <vector>
#include <map>
#include <stdexcept>


#include "event.h"

namespace md {

    class bar {
    public:

        explicit bar();

        ~bar();

        bar(const bar &other);

        bar &operator=(const bar &other);

        bar(bar &&other) noexcept;

        bar &operator=(bar &&other) noexcept;

        void set_time_signature(uint32_t beats_per_bar,
                                uint32_t quarter_notes_per_beat);


        // a vector of events at position
        // event is a vector of uint8_t
        class events : public std::vector<std::vector<uint8_t>>{
        public:
            events() = default;

            events& operator += (const std::vector<uint8_t>& event){
                emplace_back(event);
                return *this;
            }

            events& operator += (std::initializer_list<uint8_t> event){
                emplace_back(event);
                return *this;
            }
        };


        bar::events& operator[](size_t pos);

        const uint32_t beats_per_bar() const;

        const uint32_t qn_per_beat() const;

        std::map<size_t,events>& get_events_map();

        const  std::map<size_t,events>& get_events_map() const;

    private:
        uint32_t m_beats_per_bar;
        uint32_t m_quarter_notes_per_beat;
        std::map<size_t,events> m_events_map;
    };

}

#endif
