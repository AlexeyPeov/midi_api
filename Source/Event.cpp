#include "../Headers/Event.h"

namespace md{

    Event::Event(uint32_t dt){
        m_dt = dt;
    }

    Event::Event(
            uint32_t dt,
            uint16_t a,
            uint16_t b,
            uint16_t c
    )
    {
        m_dt = dt;
        if(m_good(a)){
            m_message_vec.emplace_back(a);
        }
        if(m_good(b)){
            m_message_vec.emplace_back(b);
        }
        if(m_good(c)){
            m_message_vec.emplace_back(c);
        }
    }

    uint32_t Event::dt() const {
        return m_dt;
    }

    void Event::set_dt(uint32_t dt){
        m_dt = dt;
    }

    bool Event::empty() const{
        return m_message_vec.empty();
    }

    MidiMessageType Event::get_msg_type() const {

        if (!m_message_vec.empty()){
            return static_cast<MidiMessageType>(m_message_vec[0]);
        }
        return MidiMessageType::kUndefined;
    }

    bool Event::is_meta() const {
        if (m_message_vec.size() >= 2){
            return m_message_vec[0] == 0xff;
        }
        return false;
    }

    bool Event::is_meta(MidiMetaType meta_type) const {
        return is_meta() && (m_message_vec[1] == (uint16_t)meta_type);
    }

    bool Event::is_sysex() const {
        if (!m_message_vec.empty()){
            auto& a = m_message_vec[0];
            bool first_pred = a == (uint16_t)MidiMessageType::kSysExBegin;
            bool second_pred = a == (uint16_t)MidiMessageType::kSysExEnd;
            return (first_pred || second_pred);
        }
        return false;
    }

    bool Event::is_realtime() const {
        if (!m_message_vec.empty()){
            return m_message_vec[0] >= 0xf8;
        }
        return false;
    }

    bool Event::is_voice_category() const {
        if (!m_message_vec.empty()){
            return (m_message_vec[0] >= 0x80) && (m_message_vec[0] <= 0xef);
        }
        return false;
    }

    bool Event::is_voice_category(MidiMessageType type) const {
        if (!m_message_vec.empty()){
            return (m_message_vec[0] & 0xf0) == (uint16_t)type;
        }
        return false;
    }

    bool Event::is_system_common() const {
        if (!m_message_vec.empty()){
            return (m_message_vec[0] >= 0xf0) && (m_message_vec[0] <= 0xf7);
        }
        return false;
    }

    bool Event::contains_text() const {
        if (m_message_vec.size() >= 2){
            auto& a = m_message_vec[0];
            auto& b = m_message_vec[1];
            bool kmeta = a == 0xff;
            bool second = b == (uint16_t)MidiMetaType::kText;
            bool third = b == (uint16_t)MidiMetaType::kLyrics;
            bool fourth = b == (uint16_t)MidiMetaType::kInstrumentName;
            bool fifth = b == (uint16_t)MidiMetaType::kTrackName;
            bool sixth = b == (uint16_t)MidiMetaType::kCopyright;
            bool contains = kmeta && (second||third||fourth||fifth||sixth);
            return contains;
        }
        return false;
    }

    std::string Event::get_text() const {
        std::string r;
        if (contains_text()) {
            for (size_t i = 2; i < size(); i++){
                r += static_cast<char>(m_message_vec[i]);
            }
        }
        return r;
    }

    size_t Event::size() const {
        return m_message_vec.size();
    }


    const std::vector<uint8_t> & Event::get_message_vec() const{
        return m_message_vec;
    }

    std::vector<uint8_t> & Event::get_message_vec(){
        return m_message_vec;
    }

    bool Event::m_good(uint16_t val) const {
        return val != NOT_SET;
    }
}
