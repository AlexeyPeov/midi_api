#include "../Headers/Track.h"

namespace md{

    Track::Track() = default;

    Track::Track(
            uint32_t start_octave,
            uint32_t range_in_octaves,
            ProgressionStyle progression_style,
            NoteLenBias note_len_bias,
            uint32_t length_of_notes
    )
    {
        m_start_octave = start_octave;
        m_range_in_octaves = range_in_octaves;
        m_progression = progression_style;
        m_note_bias = note_len_bias;
        m_length_of_notes = length_of_notes;
    }


    Track::~Track() = default;

    void Track::reset(){
        *this = Track();
    }

    std::vector<Bar>& Track::get_bars_vec(){
        return m_bars_vec;
    }

    const std::vector<Bar>& Track::get_bars_vec() const{
        return m_bars_vec;
    }

    uint8_t Track::get_instrument(){
        return m_instrument;
    }

    ProgressionStyle Track::get_chord_style(){
        return m_progression;
    }

    uint32_t Track::get_notes_len() {
        return m_length_of_notes;
    }

    NoteLenBias Track::get_note_len_bias() {
        return m_note_bias;
    }

    uint32_t Track::get_start_octave() {
        return m_start_octave;
    }

    uint32_t Track::get_range_in_octaves() {
        return m_range_in_octaves;
    }

    ProgressionStyle Track::get_progression_style() {
        return m_progression;
    }

    void Track::set_start_octave(uint32_t start_octave){
        m_start_octave = start_octave;
    }

    void Track::set_range_in_octaves(uint32_t range){
        m_range_in_octaves = range;
    }

    void Track::set_progression_style(ProgressionStyle progressionStyle){
        m_progression = progressionStyle;
    }

    void Track::set_note_len_bias(NoteLenBias noteLenBias){
        m_note_bias = noteLenBias;
    }

    void Track::set_length_of_notes(uint32_t length){
        m_length_of_notes = length;
    }

    void Track::set_instrument(const uint8_t instrument){
        m_instrument = instrument;
    }

}
