#ifndef MAPI_FILE_H
#define MAPI_FILE_H

#include <chrono> // NOLINT() CPP11_INCLUDES
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "IOHelper/IOHelper.h"
#include "NoteInfo/NoteInfo.h"
#include "track/track.h"

namespace md
{

class file
{
public:
    file();

    explicit file(const std::string& path);
    
    // file(file&& other) noexcept;

    // file& operator = (file&& other) noexcept;

    /// @brief
    /// @param path
    /// @return 0 if success, -1 if not
    int load(const std::string& path);

    int save(const std::string& path) const;

    const std::vector<track>& getTracks() const;

    std::vector<NoteInfo> toNoteInfoVec() const;

    uint16_t getPulsesPerQuarternote() const;

    void addTrack(const track& track);
    
    void addTrack(track&& track);

private:

    void readHeaderChunk(std::ifstream& file);

    static void readUnknownChunk(std::ifstream& file);

    void readTrackChunk(std::ifstream& file);

    static void readEvent(
        std::ifstream& file,
        event* event,
        bool* track_continue,
        uint8_t* running_status
    );

    void saveHeaderChunk(std::ofstream& output_file) const;

    static void saveTrackChunk(
        std::ofstream& output_file,
        const track& track
    );

    static uint32_t saveEvent(
        std::ofstream& output_file,
        const event& event,
        uint8_t* last_cmd
    );

    static uint32_t write(
        std::ofstream& file,
        const uint8_t* c,
        std::streamsize size = 1
    );

private:
    uint16_t m_ppq;
    std::vector<track> m_tracksVec;
};

}

#endif
