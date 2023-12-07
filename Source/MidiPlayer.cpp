#include "MidiPlayer.h"

namespace md {


    midi_player::midi_player(bool async) {
        m_tempo = 500000 / 500;
        m_file_ptr = nullptr;
        m_output = std::make_unique<DefaultMidiOutput>();
        midi_player::m_setup_windows_timers();

        m_async = async;

    }

    void midi_player::m_setup_windows_timers() {
#ifdef WIN32
        static bool once = true;
        if (once) {
            TIMECAPS tc;
            UINT timer_res;

            if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) == TIMERR_NOERROR) {
                timer_res = std::min(std::max((int) (tc.wPeriodMin), 1),
                                     (int) tc.wPeriodMax);
                timeBeginPeriod(timer_res);
            }

            once = false;
        }
#endif
    }

    void midi_player::set_file(std::unique_ptr<file> file) {

        if (!file) {
            std::cerr << "error at md::midi_player::set_file : nullptr given\n";
            return;
        }

        std::lock_guard<std::mutex> guard(m_mutex);
        m_output->reset();
        m_tempo = 500000 / 500;
        m_max_time = 0;
        m_file_ptr = std::move(file);

        auto tracks_size = m_file_ptr->get_tracks().size();
        m_tracks_pos_vec.clear();
        m_tracks_pos_vec.reserve(tracks_size);

        m_setup_async_goto_vec();

        // populate tracks info
        for (int tr_index = 0; tr_index < tracks_size; ++tr_index) {
            m_tracks_pos_vec.emplace_back();
        }

    }

    void midi_player::m_setup_async_goto_vec(){

        auto& tracks = m_file_ptr->get_tracks();
        auto tracks_size = tracks.size();

        m_track_ev_time_vec.clear();
        m_track_ev_time_vec.reserve(tracks_size);

        for(int i = 0; i < tracks_size;++i){
            m_track_ev_time_vec.emplace_back();

            size_t time = 0;

            auto& track = tracks[i];
            auto track_sz = track.size();
            auto& ev_time = m_track_ev_time_vec[i];

            ev_time.resize(track_sz);

            for(int j = 0; j < track_sz; ++j){
                time += track[j].dt();
                ev_time[j] = time;
            }

            m_max_time = std::max(m_max_time, time);
        }
    }

    void midi_player::m_execute_event(const Event &event) {
        if (event.is_meta()) {
            if (event.is_meta(MidiMetaType::kTempo)) {
                auto &msg = event.get_message_vec();
                m_tempo = IOHelper::extract_tempo(msg[2], msg[3], msg[4]);
                if (m_file_ptr) {
                    m_tempo /= m_file_ptr->get_time_division();
                }
            }
            return;  // handle other META events(i ignore them, i dont care)
        }

        if (m_output) {
            m_output->send_message(&event.get_message_vec());
        }
    }

    void midi_player::m_play_sync() {
        bool finished = false;

        while (!finished) {
            finished = m_execute_next_available_event();
        }
    }

    void midi_player::m_play_async() {
        while(m_player_state != PlayerState::NO){

            if(m_player_state == PlayerState::PLAYING){
                bool finished = m_execute_next_available_event();
                if(finished){
                    m_player_state = PlayerState::PAUSED;
                }
            }

            else if (m_player_state == PlayerState::PAUSED){
                std::this_thread::sleep_for(std::chrono::milliseconds(16));
            }
        }
    }

    void midi_player::play() {

        if(!m_file_ptr)
            return;


        if (m_async) {
            m_player_state = PlayerState::PLAYING;

            bool thread_started = m_thread.joinable();

            if(!thread_started){
                m_thread = std::thread([this] { m_play_async(); });
            }
        }
        else {

            m_play_sync();

        }
    }

    void midi_player::pause() {
        std::lock_guard<std::mutex> guard(m_mutex);
        m_player_state = PlayerState::PAUSED;
    }

    void midi_player::m_increment_pos_ptr() {

        for (int i = 0; i < m_tracks_pos_vec.size(); ++i) {

            auto& tr_info = m_tracks_pos_vec[i];

            auto &events = m_file_ptr->get_tracks()[i];

            if (tr_info.m_event_id >= events.size()) {
                continue;
            }

            auto event = events[tr_info.m_event_id];

            if (!tr_info.m_read) {
                // setting time to curr event if didn't yet read the event
                tr_info.m_time += event.dt();
                tr_info.m_read = true;
            }
        }
    }

    midi_player::EventInfo midi_player::m_get_next_event() const {

        Event *event_ptr = nullptr;
        uint32_t min_time = UINT32_MAX;
        size_t sleep_time_til_next = SIZE_MAX;
        uint32_t pos = UINT32_MAX;

        for (int i = 0; i < m_tracks_pos_vec.size(); ++i) {

            auto& p = m_tracks_pos_vec[i];

            auto &events = m_file_ptr->get_tracks()[i];

            if (p.m_event_id >= events.size()) {
                continue;
            }

            Event &e = events[p.m_event_id];
            const size_t &time = p.m_time;

            if (time <= min_time) {
                event_ptr = &e;
                min_time = time;
                pos = i;
            }
        }
        for (int i = 0; i < m_tracks_pos_vec.size(); ++i) {
            auto &p = m_tracks_pos_vec[i];
            auto &time = m_tracks_pos_vec[i].m_time;
            auto &track = m_file_ptr->get_tracks()[i];
            if (i == pos) {
                auto next_ev_pos = p.m_event_id+1;
                uint32_t next_time = UINT32_MAX;

                if(next_ev_pos < track.size()){
                    next_time = time + track[next_ev_pos].dt();
                }

                sleep_time_til_next = std::min(
                        sleep_time_til_next,
                        next_time - time
                );
                continue;
            }
            sleep_time_til_next = std::min(sleep_time_til_next,
                                           time - min_time);
        }

        sleep_time_til_next = sleep_time_til_next * m_tempo;

        if(!event_ptr){
            sleep_time_til_next = 16 * m_tempo;
        }

        return {event_ptr, sleep_time_til_next, pos};
    }

    void midi_player::go_to(double pos) {
        if(pos > 1){
            pos = 1;
        }
        else if (pos < 0){
            pos = 0;
        }

        std::lock_guard<std::mutex> guard(m_mutex);

        m_output->reset();

        for(int i = 0; i < m_tracks_pos_vec.size(); ++i){

            size_t event_index = m_get_ev_index(pos, i);

            m_tracks_pos_vec[i].m_event_id = event_index;
            m_tracks_pos_vec[i].m_read = true;
            m_tracks_pos_vec[i].m_time = m_track_ev_time_vec[i][event_index];
        }
    }

    size_t midi_player::m_get_ev_index(const double pos, int track_index){
        auto& events_time = m_track_ev_time_vec[track_index];
        size_t size = events_time.size();

        if(size == 0){
            size = 1;
        }

        size_t to_find = m_max_time * pos;

        size_t lo = 0, hi = size-1;

        while(lo < hi){

            size_t middle = lo + (hi-lo) / 2;

            auto& curr = events_time[middle];


            if(to_find < curr){
                hi = middle-1;
            }
            else if (to_find > curr){
                lo = middle + 1;
            }
            else {
                lo = middle;
                hi = middle;
            }

        }

        auto val = std::min(lo,hi);

        if(val == size - 1){
            val = size;
        }

        return val;
    }

    midi_player::~midi_player() {
        if(m_thread.joinable()){
            m_thread.join();
        }
    }

    bool midi_player::m_execute_next_available_event() {

        if(!m_file_ptr){
            return true; // finished
        }

        m_increment_pos_ptr();
        const EventInfo e_info = m_get_next_event();
        const auto &[event_ptr,sleep_time,track_id] = e_info;

        if (event_ptr) {
            m_execute_event(*event_ptr);
            m_tracks_pos_vec[track_id].m_read = false;
            m_tracks_pos_vec[track_id].m_event_id += 1;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(sleep_time));

        bool finished = event_ptr == nullptr;
        return finished;
    }

    void midi_player::join_threads() {
        m_player_state = PlayerState::NO;
        if(m_thread.joinable()){
            m_thread.join();
        }
    }

    std::unique_ptr<file> midi_player::return_file() {
        std::lock_guard<std::mutex> guard(m_mutex);
        return std::move(m_file_ptr);
    }

}