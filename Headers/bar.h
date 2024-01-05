#ifndef MIDI_API_BAR_H
#define MIDI_API_BAR_H

#include <cstdint>
#include <vector>
#include <map>
#include <cassert>

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

        size_t size() const;


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

        class quarter_notes : public std::map<size_t,events>{
        public:
            quarter_notes() = default;

            quarter_notes& operator += (const std::vector<uint8_t>& event){
                auto& events = this[0];
                events[0].emplace_back(event);
                return *this;
            }

            quarter_notes& operator += (std::initializer_list<uint8_t> event){
                auto& events = this[0];
                events[0].emplace_back(event);
                return *this;
            }
        };

        class beat : public std::vector<quarter_notes>{
        public:
            beat() = default;

            beat& operator += (const std::vector<uint8_t>& event){
                auto& events = this[0][0];
                events[0].emplace_back(event);
                return *this;
            }

            beat& operator += (std::initializer_list<uint8_t> event){
                auto& events = this[0][0];
                events[0].emplace_back(event);
                return *this;
            }
        };

        beat& operator[](size_t pos);

        const beat& operator[](size_t pos) const;

        events& last_event();

        const uint32_t beats_per_bar() const;

        const uint32_t qn_per_beat() const;

        static uint32_t get_qn_sz();

    private:

        static const uint32_t m_qn_sz = 360;


        uint32_t m_beats_per_bar;
        uint32_t m_quarter_notes_per_beat;

        std::vector<beat> m_beats;

    };

}

#endif
