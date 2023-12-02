#include "Track.h"

namespace md{

    void Track::set_instrument(const uint8_t instrument){
        m_instrument = instrument;
    }

    uint8_t Track::get_instrument() const {
        return m_instrument;
    }

}
