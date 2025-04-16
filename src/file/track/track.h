#ifndef MAPI_TRACK_H
#define MAPI_TRACK_H

#include <stdio.h>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cassert>
#include <cstdint>
#include <set>
#include <stack>

#include "event/event.h"

#include "constants.h"


#include "../bar/bar.h"
#include "../NoteInfo/NoteInfo.h"

namespace md {

    class track {
    public:
        track() = default;
        track(uint32_t time_div);

        void addBar(const bar& bar);

        void addBars(const std::vector<bar>& bars);        

        const std::vector<event>& getEvents() const;

        void emplace_back(const event& event);

        std::vector<NoteInfo> toNoteInfoVec() const;

        void clear();

    private:
        std::vector<event> m_events;
        uint32_t m_time_div;
    };

}

#endif
