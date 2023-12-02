#ifndef MAPI_FILE_H
#define MAPI_FILE_H

#include <chrono>  // NOLINT() CPP11_INCLUDES
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

#include "Track.h"
#include "IOHelper.h"

namespace md {

    class file {
    public:
        file();

        explicit file(const char *path);

        void load(const char *path);

        int save_as(const char *path) const;

        void add_tracks(size_t amount);

        uint16_t get_time_division() const;

        void set_quarter_note_len(uint16_t q_len);

        [[nodiscard]] const std::vector<Track>& get_tracks() const;

        std::vector<Track>& get_tracks();

    private:
        uint16_t m_quarter_note_len;
        std::vector<Track> m_tracks_vec;

        void read_header_chunk(std::ifstream &file);

        static void read_unknown_chunk(std::ifstream &file);

        void read_track_chunk(std::ifstream &file);

        static void read_event(
                std::ifstream &file,
                Event *event,
                bool *track_continue,
                uint8_t *running_status
        );

        void save_header_chunk(std::ofstream &output_file) const;

        static void save_track_chunk(
                std::ofstream &output_file,
                const Track &track
        ) ;

        static uint32_t save_event(
                std::ofstream &output_file,
                const Event &event,
                uint8_t *last_cmd
        ) ;

        static uint32_t write(
                std::ofstream &file,
                const uint8_t *c,
                std::streamsize size = 1
        );
    };


}

#endif
