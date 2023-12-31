#ifndef MIDI_EVENT_HPP
#define MIDI_EVENT_HPP

#include <cstdint>
#include <vector>
#include <string>

#include "constants.h"

namespace md {

    class event {
    public:

        event() = default;

        event(uint32_t dt);

        event(uint32_t dt, const std::vector<uint8_t>& msg);

        event(event&& other) noexcept;

        event(const event& e) = default;

        event& operator = (const event& e) = default;

        bool operator == (const event& other) const;

        bool operator != (const event& other) const;

        bool empty() const;

        size_t size() const;

        uint32_t dt() const;

        msg_t get_msg_type() const;

        meta_t get_meta_type() const;

        bool is_voice_category() const;

        bool is_voice_category(msg_t type) const;

        bool is_system_common() const;

        bool is_sysex() const;

        bool is_realtime() const;

        bool is_meta() const;

        bool is_meta(meta_t meta_type) const;

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
