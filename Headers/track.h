#ifndef MAPI_TRACK_H
#define MAPI_TRACK_H

#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cassert>

#include "event.h"
#include "constants.h"
#include "bar.h"

namespace md {

    class track {
    public:

        track(uint32_t time_div);

        void add_bar(const bar& bar);

        void add_bars(const std::vector<bar>& bars);

        std::vector<event>& get_events();

        const std::vector<event>& get_events() const;

        void emplace_back(const event& event);

        void clear();

    private:
        std::vector<event> m_events;
        uint32_t m_time_div;
    };

}

#endif
