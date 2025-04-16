#ifndef MIDI_EVENT_HPP
#define MIDI_EVENT_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "constants.h"
#include "enums/instrument.h"
#include "enums/meta_t.h"
#include "enums/msg_t.h"
#include "enums/note.h"

namespace md
{

class event
{
public:
    event() = default;

    event(uint32_t dt);

    event(uint32_t dt, const std::vector<uint8_t>& msg);

    event(event&& other) noexcept;

    event(const event& e) = default;

    event& operator=(const event& e) = default;

    bool operator==(const event& other) const;

    bool operator!=(const event& other) const;

    bool empty() const;

    size_t size() const;

    uint32_t dt() const;

    msg_t getMsgType() const;

    meta_t getMetaType() const;

    bool isVoiceCategory() const;

    bool isVoiceCategory(msg_t type) const;

    bool isSystemCommon() const;

    bool isSysex() const;

    bool isRealtime() const;

    bool isMeta() const;

    bool isMeta(meta_t meta_type) const;

    bool containsText() const;

    std::string getText() const;

    const std::vector<uint8_t>& getMessageVec() const;

    std::vector<uint8_t>& getMessageVec();

    void setDt(uint32_t dt);

    bool isNoteOn() const;

    bool isNoteOff() const;

    md::note getNote() const;

private:
    uint32_t m_dt = 0;
    std::vector<uint8_t> m_msgVec;
};
}

#endif
