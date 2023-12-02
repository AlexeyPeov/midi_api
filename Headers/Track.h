#ifndef MAPI_TRACK_H
#define MAPI_TRACK_H

#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>
#include <numeric>

#include "Event.h"
#include "constants.h"
#include "Bar.h"

namespace md {

    class Track : public std::vector<Event> {
    public:

        Track() = default;

        ~Track() = default;

        void set_instrument(const uint8_t instrument);

        uint8_t get_instrument() const;

    private:
        std::string m_name;
        uint8_t m_instrument = 0;
    };

}

#endif
