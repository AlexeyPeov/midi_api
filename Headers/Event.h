#ifndef MIDI_EVENT_HPP
#define MIDI_EVENT_HPP

#include <cstdint>
#include <vector>
#include <string>

#include "constants.h"

namespace md {

    class Event {
    public:

        Event() = default;

        Event(uint32_t dt);

        bool empty() const;

        size_t size() const;

        uint32_t dt() const;

        MidiMessageType get_msg_type() const;

        MidiMetaType get_meta_type() const;

        bool is_voice_category() const;

        bool is_voice_category(MidiMessageType type) const;

        bool is_system_common() const;

        bool is_sysex() const;

        bool is_realtime() const;

        bool is_meta() const;

        bool is_meta(MidiMetaType meta_type) const;

        bool contains_text() const;

        std::string get_text() const;

        const std::vector<uint8_t> &get_message_vec() const;

        std::vector<uint8_t> &get_message_vec();

        void set_dt(uint32_t dt);

    private:
        static const uint16_t NOT_SET = 0xFFFF;
        uint32_t m_dt = 0;
        std::vector<uint8_t> m_message_vec;

        bool m_good(uint16_t) const;
    };
}


#endif
