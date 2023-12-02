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

        // can only be called if player is asynchronous, otherwise no effect
        void pause();

        // can only be called if player is asynchronous, otherwise no effect
        void resume();

        // can only be called if player is asynchronous, otherwise no effect
        // joins threads.
        void reset();

        void set_file(file *file);

        void play();

    private:

        enum class PlayerState {
            NO,
            PAUSED,
            PLAYING,
        };

        struct CurrTrackPos {
            int m_track_id = 0;
            int m_event_id = 0;
            size_t m_time = 0;
            bool m_read = true;

            explicit CurrTrackPos(int track_id) {
                m_track_id = track_id;
            }
        };

        file *m_file_ptr;

        uint32_t m_tempo;  // [microseconds per quarternote]

        size_t m_curr_pos = 0;
        std::vector<CurrTrackPos> m_tracks_pos_vec;

        PlayerState m_state;

        std::unique_ptr<MessageInterface> m_output;

        std::mutex m_mutex;
        std::thread m_thread;
        bool m_async = false;

        static void m_setup_windows_timers();

        void m_execute_event(const Event &event);

        void m_player_loop();

        void m_increment_pos_ptr();

        std::tuple<Event *, size_t, uint32_t> m_get_next_event() const;

        const Event * m_get_next_event(CurrTrackPos p) const;

        void m_play_sync();

        void m_play_async();


    };


}
#endif
