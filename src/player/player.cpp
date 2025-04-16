#include "player.h"

namespace md
{

player::player(bool async)
{
    m_tempo = 500000 / 96;
    
    m_player = std::make_unique<DefaultMidiPlayer>();

    player::setupWindowsTimes();

    m_async = async;
}

uint32_t player::extractTempo(uint8_t v0, uint8_t v1, uint8_t v2)
{
    union
    {
        uint32_t one_tempo;
        uint8_t tab_tempo[3];
    };

    one_tempo = 0;

    if (is_little_endian)
    {
        tab_tempo[0] = v2;
        tab_tempo[1] = v1;
        tab_tempo[2] = v0;
    }
    else
    {
        tab_tempo[0] = v0;
        tab_tempo[1] = v1;
        tab_tempo[2] = v2;
    }

    return one_tempo;
}

void player::setupWindowsTimes()
{
#ifdef WIN32
    static bool once = true;
    if (once)
    {
        TIMECAPS tc;
        UINT timer_res;

        if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) == TIMERR_NOERROR)
        {
            timer_res = std::min(
                std::max((int)(tc.wPeriodMin), 1),
                (int)tc.wPeriodMax
            );
            timeBeginPeriod(timer_res);
        }

        once = false;
    }
#endif
}

void player::setFile(file&& file)
{

    std::lock_guard<std::mutex> guard(m_mutex);
    m_player->reset();
    m_tempo = 500000 / file.getPulsesPerQuarternote();
    m_maxTime = 0;
    m_file = std::move(file);

    auto tracks_size = m_file.getTracks().size();
    m_tracksPosVec.clear();
    m_tracksPosVec.reserve(tracks_size);

    setupAsyncGotoVec();

    // populate tracks info
    for (size_t tr_index = 0; tr_index < tracks_size; ++tr_index)
    {
        m_tracksPosVec.emplace_back();
    }
}

file player::returnFile()
{
    std::lock_guard<std::mutex> guard(m_mutex);
    return std::move(m_file);
}

void player::setBars(const std::vector<md::bar>& bars)
{
    std::vector<std::vector<md::bar>> b = { bars };
    setBars(b);
}

void player::setBars(const std::vector<std::vector<md::bar>>& file_)
{

    md::file file;

    std::vector<md::track> fileTracks;

    for (const std::vector<md::bar>& barsVec : file_)
    {
        md::track track;

        for (const bar& bar : barsVec)
        {
            track.addBar(bar);
        }

        file.addTrack(std::move(track));
    
    }

    setFile(std::move(file));
}

void player::setupAsyncGotoVec()
{

    auto& tracks = m_file.getTracks();
    auto tracks_size = tracks.size();

    m_trackEvTimeVec.clear();
    m_trackEvTimeVec.reserve(tracks_size);

    for (size_t i = 0; i < tracks_size; ++i)
    {
        m_trackEvTimeVec.emplace_back();

        size_t time = 0;

        auto& track = tracks[i].getEvents();
        auto track_sz = track.size();
        auto& ev_time = m_trackEvTimeVec[i];

        ev_time.resize(track_sz);

        for (size_t j = 0; j < track_sz; ++j)
        {
            time += track[j].dt();
            ev_time[j] = time;
        }

        m_maxTime = std::max(m_maxTime, time);
    }
}

void player::executeEvent(const event& event)
{
    if (event.isMeta())
    {
        if (event.isMeta(meta_t::Tempo))
        {
            auto& msg = event.getMessageVec();
            m_tempo = extractTempo(msg[2], msg[3], msg[4]);
            
            m_tempo /= m_file.getPulsesPerQuarternote();
        }
        return; // handle other META events(i ignore them, i dont care)
    }

    if (m_player)
    {
        m_player->sendMessage(&event.getMessageVec());
    }
}

void player::playSync()
{
    bool finished = false;

    while (!finished)
    {
        finished = executeNextAvailableEvent();
    }
}

void player::playAsync()
{
    while (m_playerState != PlayerState::NO)
    {

        if (m_playerState == PlayerState::PLAYING)
        {
            bool finished = executeNextAvailableEvent();

            if (finished)
            {
                m_playerState = PlayerState::PAUSED;
            }
        }

        else if (m_playerState == PlayerState::PAUSED)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }
}

void player::play()
{

    if (m_async)
    {
        m_playerState = PlayerState::PLAYING;

        bool thread_started = m_thread.joinable();

        if (!thread_started)
        {
            m_thread = std::thread(
                [this]
                {
                    playAsync();
                }
            );
        }
    }
    else
    {

        playSync();
    }
}

void player::pause()
{
    std::lock_guard<std::mutex> guard(m_mutex);
    m_playerState = PlayerState::PAUSED;
}

void player::incrPosPtr()
{

    for (size_t i = 0; i < m_tracksPosVec.size(); ++i)
    {

        CurrTrackPos& trackPosInfo = m_tracksPosVec[i];

        const std::vector<md::event>& events = m_file.getTracks()[i].getEvents();

        if (trackPosInfo.m_eventId >= events.size())
        {
            continue;
        }

        const md::event& event = events[trackPosInfo.m_eventId];

        if (!trackPosInfo.m_read)
        {
            // setting time to curr event if didn't yet read the event
            trackPosInfo.m_time += event.dt();
            trackPosInfo.m_read = true;
        }
    }
}

player::EventInfo player::getNextEvent() const
{

    const event* event_ptr = nullptr;
    uint32_t min_time = UINT32_MAX;
    size_t sleep_time_til_next = SIZE_MAX;
    uint32_t pos = UINT32_MAX;

    auto& tracks = m_file.getTracks();

    // get event, it is possible that here an event won't be found
    for (size_t i = 0; i < m_tracksPosVec.size(); ++i)
    {

        auto& p = m_tracksPosVec[i];

        auto& events = tracks[i].getEvents();

        if (p.m_eventId >= events.size())
        {
            continue;
        }

        const event& e = events[p.m_eventId];
        const size_t& time = p.m_time;

        if (time <= min_time)
        {
            event_ptr = &e;
            min_time = time;
            pos = i;
        }
    }

    for (size_t i = 0; i < m_tracksPosVec.size(); ++i)
    {
        auto& p = m_tracksPosVec[i];
        auto& time = m_tracksPosVec[i].m_time;
        auto& track = m_file.getTracks()[i].getEvents();

        if (i == pos)
        {
            auto next_ev_pos = p.m_eventId + 1;
            size_t next_time = UINT32_MAX;

            if (next_ev_pos < track.size())
            {
                next_time = track[next_ev_pos].dt();
            }

            sleep_time_til_next = std::min(sleep_time_til_next, next_time);
            continue;
        }

        sleep_time_til_next = std::min(sleep_time_til_next, time - min_time);
    }

    if (!event_ptr || sleep_time_til_next == UINT32_MAX)
    {
        sleep_time_til_next = 16 * m_tempo;
    }
    else
    {
        sleep_time_til_next = sleep_time_til_next * m_tempo;
    }    

    return { event_ptr, sleep_time_til_next, pos };
}

void player::goTo(double pos)
{
    pos = std::clamp(pos, 0.0, 1.0);
    
    std::lock_guard<std::mutex> guard(m_mutex);

    m_player->reset();

    for (size_t i = 0; i < m_tracksPosVec.size(); ++i)
    {
        const std::vector<size_t>& tracksTimes = m_trackEvTimeVec[i];

        size_t eventIndex = getEventIndex(pos, i);

        if(eventIndex >= tracksTimes.size())
        {
            std::cerr << "md::player::goTo getEventIndex out of bounds: " << eventIndex << '\n';
            continue;
        }

        m_tracksPosVec[i].m_eventId = eventIndex;
        m_tracksPosVec[i].m_read = true;

        m_tracksPosVec[i].m_time = tracksTimes.at(eventIndex);
    }
}

size_t player::getEventIndex(double pos, int trackIndex)
{
    const std::vector<size_t>& eventsTime = m_trackEvTimeVec[trackIndex];

    size_t eventTimepoint = m_maxTime * pos;

    auto it = std::lower_bound(
        eventsTime.begin(), 
        eventsTime.end(), 
        eventTimepoint
    );

    if(it == eventsTime.end())
    {
        return eventsTime.size();
    }

    return std::distance(eventsTime.begin(), it);
}

player::~player()
{
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

bool player::executeNextAvailableEvent()
{

    incrPosPtr();
    
    const EventInfo e_info = getNextEvent();
    const auto& [event_ptr, sleep_time, track_id] = e_info;

    if (event_ptr)
    {
        executeEvent(*event_ptr);
        m_tracksPosVec[track_id].m_read = false;
        m_tracksPosVec[track_id].m_eventId += 1;
    }

    std::this_thread::sleep_for(std::chrono::microseconds(sleep_time));

    bool finished = event_ptr == nullptr;
    return finished;
}

void player::stop()
{
    m_playerState = PlayerState::NO;
    
    goTo(0);

    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

}