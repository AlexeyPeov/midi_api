#ifndef MAPI_FILE_H
#define MAPI_FILE_H

#include <chrono>  // NOLINT() CPP11_INCLUDES
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

#include "track.h"
#include "IOHelper.h"

namespace md {

    class file {
    public:

        file();

        explicit file(const char *path);

        int load(const std::string& path);

        int save_as(const std::string& path) const;

        [[nodiscard]] const std::vector<track>& get_tracks() const;

        std::vector<track>& get_tracks();

        uint16_t get_time_div() const;

    private:

        uint16_t m_tive_div;
        std::vector<track> m_tracks_vec;

        void read_header_chunk(std::ifstream &file);

        static void read_unknown_chunk(std::ifstream &file);

        void read_track_chunk(std::ifstream &file);

        static void read_event(
                std::ifstream &file,
                event *event,
                bool *track_continue,
                uint8_t *running_status
        );

        void save_header_chunk(std::ofstream &output_file) const;

        static void save_track_chunk(
                std::ofstream &output_file,
                const track &track
        ) ;

        static uint32_t save_event(
                std::ofstream &output_file,
                const event &event,
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
