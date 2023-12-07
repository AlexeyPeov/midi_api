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

namespace md {

    class Track : public std::vector<Event> {
    public:

        Track() = default;

        ~Track() = default;

    };

}

#endif
