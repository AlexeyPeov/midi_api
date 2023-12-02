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

    void midi_player::set_file(file *file) {

        if(!file){
            std::cerr << "error at md::midi_player::set_file : nullptr given\n";
            return;
        }

        std::lock_guard<std::mutex> guard(m_mutex);
        m_file_ptr = file;
        m_output->reset();


        auto& tracks = m_file_ptr->get_tracks();
        auto tracks_size = tracks.size();
        m_tracks_pos_vec.clear();
        m_tracks_pos_vec.reserve(tracks_size);

        // populate tracks info
        for(int tr_index = 0; tr_index < tracks_size; ++tr_index){
            m_tracks_pos_vec.emplace_back(tr_index);
        }

     }




    void midi_player::m_execute_event(const Event &event) {
        if (event.is_meta()) {
            if (event.is_meta(MidiMetaType::kTempo)) {
                auto &msg = event.get_message_vec();
                m_tempo = IOHelper::extract_tempo(msg[2], msg[3], msg[4]);
                if(m_file_ptr){
                    m_tempo /= m_file_ptr->get_time_division();
                }
            }
            return;  // handle other META events(i ignore them, i dont care)
        }

        if (m_output){
            m_output->send_message(&event.get_message_vec());
        }
    }

    void midi_player::m_play_sync() {

        if(!m_file_ptr){
            return;
        }

        bool finished = false;

        while(!finished){

            m_increment_pos_ptr();

            const auto& [event_ptr, sleep_time, el_id] = m_get_next_event();

            if(event_ptr){
                m_execute_event(*event_ptr);
                m_tracks_pos_vec[el_id].m_read = false;
                m_tracks_pos_vec[el_id].m_event_id += 1;
            }
            else{
                finished = true;
            }

            std::this_thread::sleep_for(std::chrono::microseconds(sleep_time));
        }
    }

    void midi_player::m_play_async() {

    }

    void midi_player::play() {

        if(m_async){
            m_play_async();
        }
        else {
            m_play_sync();
        }


       /* m_state = PlayerState::PLAYING;
        while (m_state != PlayerState::NO) {
            if (!m_file_ptr) {
                std::cout << "buffer is empty, or no file to play at midi_player::play\n";
                bool file_appeared = false;
                while (!m_file_ptr) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(16));

                    if (m_state == PlayerState::NO){
                        return;
                    }
                    if (m_file_ptr){
                        file_appeared = true;
                    }

                    if (file_appeared) {
                        std::cout << "good to go at Player::play\n";
                        break;
                    }
                }
            }
            m_player_loop();
        }*/
    }

    void midi_player::m_player_loop() {

        bool should_return = false;

        {
            std::lock_guard<std::mutex> guard(m_mutex);

            if (m_state != PlayerState::PLAYING){
                should_return = true;
                goto back;
            }

            m_increment_pos_ptr();
        }

        back:
        size_t sleep_time = 16;
        if (should_return) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
            return;
        }


        const auto& [event_ptr, sleep_time_til_next, el_id] = m_get_next_event();

        if(event_ptr){
            m_execute_event(*event_ptr);
            m_tracks_pos_vec[el_id].m_read = false;
            m_tracks_pos_vec[el_id].m_event_id += 1;
            sleep_time = sleep_time_til_next;

        }

        std::this_thread::sleep_for(std::chrono::microseconds(sleep_time));
    }

    void midi_player::pause() {

    }

    void midi_player::resume() {

    }

    void midi_player::reset() {

    }

    void midi_player::m_increment_pos_ptr() {
        for(auto& tr_info : m_tracks_pos_vec){

            auto& events = m_file_ptr->get_tracks()[tr_info.m_track_id];

            if(tr_info.m_event_id >= events.size()){
                continue;
            }

            auto event = events[tr_info.m_event_id];

            if(!tr_info.m_read){
                // setting time to curr event if didn't yet read the event
                tr_info.m_time += event.dt();
                tr_info.m_read = true;
            }
        }
    }

    std::tuple<Event*, size_t, uint32_t> midi_player::m_get_next_event() const

    {
        Event* event = nullptr;
        uint32_t min_time = UINT32_MAX;
        size_t sleep_time_til_next = SIZE_MAX;
        uint32_t pos = UINT32_MAX;

        for(auto& p : m_tracks_pos_vec){

            auto& events = m_file_ptr->get_tracks()[p.m_track_id];

            if(p.m_event_id >= events.size()){
                continue;
            }

            Event& e = events[p.m_event_id];
            const size_t& time = p.m_time;

            if(time <= min_time){
                event = &e;
                min_time = time;
                pos = p.m_track_id;
            }
        }

        //std::cout << "\nTRACKS INFO\n";
        for(int i = 0; i < m_tracks_pos_vec.size(); ++i){
            auto& p = m_tracks_pos_vec[i];
            //std::cout << p.m_track_id << ' ' << p.m_bar_id << ' ' <<  p.m_event_id << ' ' << p.m_time << '\n';
            auto& time = m_tracks_pos_vec[i].m_time;

            if(i == pos){
                auto next_ev = m_get_next_event(p);

                uint32_t next_time = next_ev ? time + next_ev->dt() : UINT32_MAX;

                sleep_time_til_next = std::min(
                        sleep_time_til_next,
                        next_time - time
                );
                continue;
            }
            sleep_time_til_next = std::min(sleep_time_til_next, time - min_time);
        }

        sleep_time_til_next = sleep_time_til_next * m_tempo;

        return { event, sleep_time_til_next, pos };
    }




    const Event * midi_player::m_get_next_event(
           midi_player::CurrTrackPos p
    ) const
    {
        p.m_event_id++;
        auto& track = m_file_ptr->get_tracks()[p.m_track_id];

        if(p.m_event_id < track.size()){
            return &track[p.m_event_id];
        }

        return nullptr;
    }


}