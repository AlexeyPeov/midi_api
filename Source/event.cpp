#include "event.h"

namespace md{

    event::event(uint32_t dt){
        m_dt = dt;
    }

    event::event(uint32_t dt, const std::vector<uint8_t> &msg) {
        m_dt = dt;
        m_message_vec = msg;
    }

    event::event(event &&other)  noexcept {
        m_message_vec = std::move(other.m_message_vec);
        m_dt = other.dt();
    }

    uint32_t event::dt() const {
        return m_dt;
    }

    void event::set_dt(uint32_t dt){
        m_dt = dt;
    }

    bool event::empty() const{
        return m_message_vec.empty();
    }

    msg_t event::get_msg_type() const {

        if (!m_message_vec.empty()){
            return static_cast<msg_t>(m_message_vec[0]);
        }
        return msg_t::UNDEFINED;
    }

    bool event::is_meta() const {
        if (m_message_vec.size() >= 2){
            return m_message_vec[0] == 0xff;
        }
        return false;
    }

    bool event::is_meta(meta_t meta_type) const {
        return is_meta() && (m_message_vec[1] == (uint16_t)meta_type);
    }

    meta_t event::get_meta_type() const {
        if (is_meta()) {
            return static_cast<meta_t>(m_message_vec[1]);
        }
        return meta_t::NO;
    }


    bool event::is_sysex() const {
        if (!m_message_vec.empty()){
            auto& a = m_message_vec[0];
            bool first_pred = a == (uint16_t)msg_t::SysExBegin;
            bool second_pred = a == (uint16_t)msg_t::SysExEnd;
            return (first_pred || second_pred);
        }
        return false;
    }

    bool event::is_realtime() const {
        if (!m_message_vec.empty()){
            return m_message_vec[0] >= 0xf8;
        }
        return false;
    }

    bool event::is_voice_category() const {
        if (!m_message_vec.empty()){
            return (m_message_vec[0] >= 0x80) && (m_message_vec[0] <= 0xef);
        }
        return false;
    }

    bool event::is_voice_category(msg_t type) const {
        if (!m_message_vec.empty()){
            return (m_message_vec[0] & 0xf0) == (uint16_t)type;
        }
        return false;
    }

    bool event::is_system_common() const {
        if (!m_message_vec.empty()){
            return (m_message_vec[0] >= 0xf0) && (m_message_vec[0] <= 0xf7);
        }
        return false;
    }

    bool event::contains_text() const {
        if (m_message_vec.size() >= 2){
            auto& a = m_message_vec[0];
            auto& b = m_message_vec[1];
            bool kmeta = a == 0xff;
            bool second = b == (uint16_t)meta_t::Text;
            bool third = b == (uint16_t)meta_t::Lyrics;
            bool fourth = b == (uint16_t)meta_t::InstrumentName;
            bool fifth = b == (uint16_t)meta_t::TrackName;
            bool sixth = b == (uint16_t)meta_t::Copyright;
            bool contains = kmeta && (second||third||fourth||fifth||sixth);
            return contains;
        }
        return false;
    }

    std::string event::get_text() const {
        std::string r;
        if (contains_text()) {
            for (size_t i = 2; i < size(); i++){
                r += static_cast<char>(m_message_vec[i]);
            }
        }
        return r;
    }

    size_t event::size() const {
        return m_message_vec.size();
    }


    const std::vector<uint8_t> & event::get_message_vec() const{
        return m_message_vec;
    }

    std::vector<uint8_t> & event::get_message_vec(){
        return m_message_vec;
    }

    bool event::m_good(uint16_t val) const {
        return val != NOT_SET;
    }

    bool event::operator==(const event &other) const {
        return (m_dt == other.m_dt) && (m_message_vec == other.m_message_vec);
    }

    bool event::operator!=(const event &other) const {
        return !(*this == other);
    }


}
