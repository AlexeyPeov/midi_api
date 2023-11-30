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

        Event(
                uint32_t dt,
                uint16_t a = NOT_SET,
                uint16_t b = NOT_SET,
                uint16_t c = NOT_SET
        );

        [[nodiscard]] bool empty() const;

        [[nodiscard]] size_t size() const;

        [[nodiscard]] uint32_t dt() const;

        [[nodiscard]] MidiMessageType get_msg_type() const;

        [[nodiscard]] bool is_voice_category() const;

        [[nodiscard]] bool is_voice_category(MidiMessageType type) const;

        [[nodiscard]] bool is_system_common() const;

        [[nodiscard]] bool is_sysex() const;

        [[nodiscard]] bool is_realtime() const;

        [[nodiscard]] bool is_meta() const;

        [[nodiscard]] bool is_meta(MidiMetaType meta_type) const;

        [[nodiscard]] bool contains_text() const;

        [[nodiscard]] std::string get_text() const;

        [[nodiscard]] const std::vector<uint8_t> & get_message_vec() const;

        std::vector<uint8_t> & get_message_vec();

        void set_dt(uint32_t dt);

    private:
        static const uint16_t NOT_SET = 0xFFFF;
        uint32_t m_dt = 0;
        std::vector<uint8_t> m_message_vec;

        [[nodiscard]] bool m_good(uint16_t) const;
    };
}


#endif
