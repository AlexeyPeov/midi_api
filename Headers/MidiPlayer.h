#ifndef MIDI_API_MIDIPLAYER_H
#define MIDI_API_MIDIPLAYER_H

#include "DefaultMidiOutput.h"

#include <cassert>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <functional>
#include <vector>
#include <ntdef.h>
#include <commctrl.h>
#include <thread>
#include <mutex>
#include <memory>


#include "File.h"

namespace md {
    class midi_player {
    public:

        // set to true if you want it to play on a different thread
        explicit midi_player(bool async = false);

        ~midi_player();

        // can only be called if player is asynchronous, otherwise no effect
        void pause();

        void set_file(std::unique_ptr<file> file);

        std::unique_ptr<file> return_file();

        void play();

        // pos should be between 0 and 1
        void go_to(double pos);

        void join_threads();

    private:

        enum class PlayerState {
            NO,
            PAUSED,
            PLAYING,
        };

        struct CurrTrackPos {
            size_t m_event_id = 0;
            size_t m_time = 0;
            bool m_read = false;

            CurrTrackPos() = default;

        };

        struct EventInfo{
            Event* m_event_ptr = nullptr;
            size_t m_sleep_time = 0;
            size_t m_track_id = 0;
        };

        std::unique_ptr<file> m_file_ptr;

        uint32_t m_tempo;

        size_t m_max_time = 0;
        std::vector<std::vector<size_t>> m_track_ev_time_vec;

        std::vector<CurrTrackPos> m_tracks_pos_vec; // for sync playing

        PlayerState m_player_state = PlayerState::NO;

        std::unique_ptr<MessageInterface> m_output;

        std::mutex m_mutex;
        std::thread m_thread;
        bool m_async = false;

        static void m_setup_windows_timers();

        void m_setup_async_goto_vec();

        void m_execute_event(const Event &event);

        void m_player_loop();

        void m_increment_pos_ptr();

        size_t m_get_ev_index(const double pos, int track_index);

        EventInfo m_get_next_event() const;

        void m_play_sync();

        void m_play_async();

        bool m_execute_next_available_event();
    };


}
#endif
