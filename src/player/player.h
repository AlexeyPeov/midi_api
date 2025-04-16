#ifndef MIDI_API_MIDI_PLAYER_H
#define MIDI_API_MIDI_PLAYER_H

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "DefaultMidiPlayer/DefaultMidiPlayer.h"

#include "../file/file.h"

namespace md
{
class player
{
public:
    // set to true if you want it to play on a different thread
    explicit player(bool async = false);

    ~player();

    // can only be called if player is asynchronous, otherwise no effect
    void pause();

    void setFile(md::file&& file);

    md::file returnFile();

    /// @brief single track
    void setBars(const std::vector<md::bar>& bars);

    /// @brief multiple tracks
    void setBars(const std::vector<std::vector<md::bar>>& bars);

    void play();

    // pos should be between 0 and 1
    void goTo(double pos);

    void stop();

private:
    enum class PlayerState
    {
        NO,
        PAUSED,
        PLAYING,
    };

    struct CurrTrackPos
    {
        size_t m_eventId = 0;
        size_t m_time = 0;
        bool m_read = false;

        CurrTrackPos() = default;
    };

    struct EventInfo
    {
        const event* m_eventPtr = nullptr;
        size_t m_sleepTime = 0;
        size_t m_trackId = 0;
    };

private:

    static uint32_t extractTempo(uint8_t v0, uint8_t v1, uint8_t v2);

    static void setupWindowsTimes();

    void setupAsyncGotoVec();

    void executeEvent(const event& event);

    void incrPosPtr();

    size_t getEventIndex(double pos, int track_index);

    EventInfo getNextEvent() const;

    void playSync();

    void playAsync();

    bool executeNextAvailableEvent();

private:
    md::file m_file;

    uint32_t m_tempo;

    size_t m_maxTime = 0;

    std::vector<std::vector<size_t>> m_trackEvTimeVec;

    std::vector<CurrTrackPos> m_tracksPosVec; // for sync playing

    std::atomic<PlayerState> m_playerState = PlayerState::NO;

    std::unique_ptr<MessageInterface> m_player;

    std::mutex m_mutex;
    std::thread m_thread;
    bool m_async = false;
};

}
#endif
