#include "event.h"

namespace md{

    md::event::event(uint32_t dt)
    {
        m_dt = dt;
    }

    md::event::event(uint32_t dt, const std::vector<uint8_t> &msg) {
        m_dt = dt;
        m_msgVec = msg;
    }

    md::event::event(event &&other)  noexcept {
        m_msgVec = std::move(other.m_msgVec);
        m_dt = other.dt();
    }

    uint32_t event::dt() const {
        return m_dt;
    }

    void event::setDt(uint32_t dt){
        m_dt = dt;
    }

    bool event::isNoteOn() const
    {
        return getMsgType() >= msg_t::NoteOn && getMsgType() <= msg_t::NoteOnEnd && m_msgVec[2] != 0;
    }

    bool event::isNoteOff() const
    {
        return (getMsgType() >= msg_t::NoteOff && getMsgType() <= msg_t::NoteOffEnd) ||(getMsgType() >= msg_t::NoteOn && getMsgType() <= msg_t::NoteOnEnd && m_msgVec[2] == 0);
    }

    md::note event::getNote() const
    {
        if(isNoteOff() || isNoteOn()){
            return md::note(m_msgVec[1]);
        }
        
        return md::note::UNDEFINED_NOTE;
    }

    bool event::empty() const{
        return m_msgVec.empty();
    }

    msg_t event::getMsgType() const {

        if (!m_msgVec.empty()){
            return static_cast<msg_t>(m_msgVec[0]);
        }
        return msg_t::UNDEFINED_MSG_T;
    }

    bool event::isMeta() const {
        if (m_msgVec.size() >= 2){
            return m_msgVec[0] == 0xff;
        }
        return false;
    }

    bool event::isMeta(meta_t meta_type) const {
        return isMeta() && (m_msgVec[1] == (uint16_t)meta_type);
    }

    meta_t event::getMetaType() const {
        if (isMeta()) {
            return static_cast<meta_t>(m_msgVec[1]);
        }
        return meta_t::NO;
    }


    bool event::isSysex() const {
        if (!m_msgVec.empty()){
            auto& a = m_msgVec[0];
            bool first_pred = a == (uint16_t)msg_t::SysExBegin;
            bool second_pred = a == (uint16_t)msg_t::SysExEnd;
            return (first_pred || second_pred);
        }
        return false;
    }

    bool event::isRealtime() const {
        if (!m_msgVec.empty()){
            return m_msgVec[0] >= 0xf8;
        }
        return false;
    }

    bool event::isVoiceCategory() const {
        if (!m_msgVec.empty()){
            return (m_msgVec[0] >= 0x80) && (m_msgVec[0] <= 0xef);
        }
        return false;
    }

    bool event::isVoiceCategory(msg_t type) const {
        if (!m_msgVec.empty()){
            return (m_msgVec[0] & 0xf0) == (uint16_t)type;
        }
        return false;
    }

    bool event::isSystemCommon() const {
        if (!m_msgVec.empty()){
            return (m_msgVec[0] >= 0xf0) && (m_msgVec[0] <= 0xf7);
        }
        return false;
    }

    bool event::containsText() const {
        if (m_msgVec.size() >= 2){
            auto& a = m_msgVec[0];
            auto& b = m_msgVec[1];
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

    std::string event::getText() const {
        std::string r;
        if (containsText()) {
            for (size_t i = 2; i < size(); i++){
                r += static_cast<char>(m_msgVec[i]);
            }
        }
        return r;
    }

    size_t event::size() const {
        return m_msgVec.size();
    }


    const std::vector<uint8_t> & event::getMessageVec() const{
        return m_msgVec;
    }

    std::vector<uint8_t> & event::getMessageVec(){
        return m_msgVec;
    }

    bool event::operator==(const event &other) const
    {
        return (m_dt == other.m_dt) && (m_msgVec == other.m_msgVec);
    }

    bool event::operator!=(const event &other) const {
        return !(*this == other);
    }


}
