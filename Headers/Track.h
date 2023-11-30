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

    class Track {
    public:

        Track();

        Track(
                uint32_t start_octave,
                uint32_t range_in_octaves,
                ProgressionStyle progression_style,
                NoteLenBias note_len_bias,
                uint32_t length_of_notes
        );

        ~Track();

        void reset();

        void set_start_octave(uint32_t start_octave);

        void set_range_in_octaves(uint32_t range);

        void set_progression_style(ProgressionStyle progressionStyle);

        void set_note_len_bias(NoteLenBias noteLenBias);

        void set_length_of_notes(uint32_t length);

        void set_instrument(const uint8_t instrument);

        uint8_t get_instrument();

        uint32_t get_notes_len();

        NoteLenBias get_note_len_bias();

        uint32_t get_start_octave();

        uint32_t get_range_in_octaves();

        size_t buffer_size();

        std::vector<Bar> &get_bars_vec();

        [[nodiscard]] const std::vector<Bar> &get_bars_vec() const;

        ProgressionStyle get_progression_style();

        ProgressionStyle get_chord_style();

    private:
        std::string m_name;
        std::vector<Bar> m_bars_vec;

        uint32_t m_start_octave = 1;
        uint32_t m_range_in_octaves = 1;
        uint32_t m_length_of_notes = 0; // 0 - random
        uint8_t m_instrument = 0;

        ProgressionStyle m_progression = ProgressionStyle::NOTES;
        NoteLenBias m_note_bias = NoteLenBias::NO;
    };

}

#endif
