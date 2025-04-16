#include "file.h"

namespace md
{

uint32_t file::write(
    std::ofstream& file,
    const uint8_t* c,
    std::streamsize size
)
{
    file.write(reinterpret_cast<const char*>(c), size);
    return size;
}

file::file()
{
    m_ppq = 96;
}

file::file(const std::string& path)
{
    m_ppq = 96;
    load(path);
}

// file::file(file&& other) noexcept
// {
//     m_ppq = std::move(other.m_ppq);
//     m_tracksVec = std::move(other.m_tracksVec);
// }

// file& file::operator=(file&& other) noexcept
// {
//     if (this != &other)
//     {
//         m_ppq = std::move(other.m_ppq);
//         m_tracksVec = std::move(other.m_tracksVec);

//         other.m_ppq = 0;
//         other.m_tracksVec.clear();
//     }
//     return *this;
// }

int file::save(const std::string& path) const
{
    std::ofstream out_file(path, std::ios_base::binary);

    if (!out_file.good())
    {
        std::cerr << "file::saveToDir could not open file " << path << '\n';
        return -1;
    }

    saveHeaderChunk(out_file);

    for (const auto& track : m_tracksVec)
    {
        saveTrackChunk(out_file, track);
    }

    out_file.close();
    return 0;
}

const std::vector<track>& file::getTracks() const
{
    return m_tracksVec;
}

std::vector<NoteInfo> file::toNoteInfoVec() const
{
    std::vector<NoteInfo> ret;

    for (auto& track : m_tracksVec)
    {
        auto trackNoteInfos = track.toNoteInfoVec();
        ret.insert(ret.end(), trackNoteInfos.begin(), trackNoteInfos.end());
    }

    std::sort(
        ret.begin(),
        ret.end(),
        [](const NoteInfo& a, const NoteInfo& b)
        {
            return a.start < b.start;
        }
    );

    return ret;
}

void file::saveTrackChunk(std::ofstream& output_file, const track& track)
{

    IOHelper::writeAs<uint32_t>(output_file, 0x4D54726B); // "MTrk"

    auto size_pos = output_file.tellp();

    IOHelper::writeAs<uint32_t>(output_file, 0);

    uint32_t chunk_size = 0;
    uint8_t last_cmd = 0;

    for (const auto& e : track.getEvents())
    {
        chunk_size += saveEvent(output_file, e, &last_cmd);
    }

    // end of track

    if (track.getEvents().empty()
        || track.getEvents().back()
            != event(0, { msg_t::Meta, meta_t::EndOfTrack, 0 }))
    {
        chunk_size += saveEvent(
            output_file,
            event(0, { msg_t::Meta, meta_t::EndOfTrack, 0 }),
            &last_cmd
        );
    }

    output_file.seekp(size_pos);

    IOHelper::writeAs<uint32_t>(output_file, chunk_size);

    output_file.seekp(0, std::ios_base::end);
}

uint32_t file::saveEvent(
    std::ofstream& output_file,
    const event& event,
    uint8_t* last_cmd
)
{

    uint32_t r = IOHelper::saveVariableLenQuantity(output_file, event.dt());

    if (event.empty())
    {
        std::cout << "EVENT EMPTY AT FILE::SAVE_EVENT\n";
        return r;
    }

    int skip_data_bytes;
    if (event.isSysex())
    {
        auto first = event.getMessageVec().at(0);
        r += write(output_file, &first);

        uint8_t data_size = static_cast<uint8_t>(event.size()) - 1;
        r += IOHelper::saveVariableLenQuantity(output_file, data_size);

        skip_data_bytes = 1;
        *last_cmd = 0;
    }
    else if (event.isMeta())
    {
        auto data = event.getMessageVec().data();
        r += write(output_file, data, 2); // byte 0 and 1

        uint8_t data_size = static_cast<uint8_t>(event.size()) - 2;
        r += write(output_file, &data_size); // save length

        skip_data_bytes = 2;
        *last_cmd = 0;
    }
    else
    {
        const uint8_t nowCmd = event.getMessageVec()[0];
        skip_data_bytes = (*last_cmd == nowCmd);
        *last_cmd = nowCmd; // save current command if we use running status
    }

    r += write(
        output_file,
        event.getMessageVec().data() + skip_data_bytes,
        event.getMessageVec().size() - skip_data_bytes
    );
    return r;
}

void file::saveHeaderChunk(std::ofstream& output_file) const
{

    IOHelper::writeAs<uint32_t>(output_file, 0x4D546864); // "MThd"

    // save header size
    IOHelper::writeAs<uint32_t>(output_file, 6);

    // save file type
    auto tr_sz = m_tracksVec.size();
    uint16_t fileType = (tr_sz > 1) ? 1 : 0;
    IOHelper::writeAs<uint16_t>(output_file, fileType);

    // save tracks number
    IOHelper::writeAs<uint16_t>(output_file, static_cast<uint16_t>(tr_sz));

    // save time division
    IOHelper::writeAs<uint16_t>(output_file, m_ppq);
}

int file::load(const std::string& path)
{
    std::ifstream file(path, std::ifstream::in | std::ifstream::binary);

    if (!file.is_open())
    {
        std::cerr << "mapi::file::load couldn't open: " << path << '\n';
        return -1;
    }

    m_tracksVec.clear();

    // calculate file length
    file.seekg(0, std::ifstream::end);
    auto file_len = file.tellg();

    file.seekg(0, std::ifstream::beg);

    // control counters
    uint32_t i = 0;
    uint32_t headers = 0;

    // loop over chunks while data still in buffer

    const auto msg_type_header = 0x4D546864;
    const auto msg_type_track = 0x4D54726B;

    while (file.good() && (file_len - file.tellg()))
    {
        auto chunk_id = IOHelper::readAs<uint32_t>(file);

        switch (chunk_id)
        {
        case msg_type_header:
            readHeaderChunk(file);
            headers++;
            break;

        case msg_type_track:
            readTrackChunk(file);
            break;

        default:
            std::cerr << "mapi::file::load ignoring unknown chunk: 0x"
                      << std::hex << static_cast<int>(chunk_id) << '\n';
            readUnknownChunk(file);
            break;
        }

        if (!i++ && !headers)
        {
            std::cerr << "mapi::file::load no header chunk "
                         "(probably not a MIDI file)"
                      << '\n';
            // *this = md::file();

            return -1;
        }
    }

    return 0;
}

void file::readHeaderChunk(std::ifstream& file)
{
    // read and check header size
    if (IOHelper::readAs<uint32_t>(file) != 6)
    {
        std::cerr << "unsupported MIDI file type" << '\n';
    }

    // read file type
    auto type = IOHelper::readAs<uint16_t>(file);

    // check file type
    if ((type != 0) && (type != 1))
        std::cerr << "unsupported MIDI file type: " << type << '\n';
    // type 0: single track
    // type 1: multi-track

    // read tracks number
    auto tracks_num = IOHelper::readAs<uint16_t>(file);

    m_tracksVec.reserve(tracks_num);

    // read time division
    m_ppq = IOHelper::readAs<uint16_t>(file);
 
    // check time division
    if (m_ppq & 0x8000)
    {
        std::cerr << "unsupported MIDI file time division" << '\n';
    }
}

void file::readUnknownChunk(std::ifstream& file)
{
    auto chunk_size = IOHelper::readAs<uint32_t>(file);
    // skip chunk data
    file.seekg(chunk_size, std::ifstream::cur);
}

void file::readTrackChunk(std::ifstream& file)
{
    m_tracksVec.emplace_back(m_ppq);
    track& track = m_tracksVec.back();

    auto chunk_size = IOHelper::readAs<uint32_t>(file);

    uint8_t running_status = 0;
    auto begin = file.tellg();
    bool track_continue = true;

    while (track_continue)
    {
        event event;

        uint32_t dt = IOHelper::getVariableLenQuantity(file);

        event.setDt(dt);

        readEvent(file, &event, &track_continue, &running_status);

        track.emplace_back(event);
    }

    if (chunk_size != (file.tellg() - begin))
        std::cerr << "track data and track chunk size mismatch" << '\n';
}

void file::readEvent(
    std::ifstream& file,
    event* event,
    bool* track_continue,
    uint8_t* running_status
)
{
    auto cmd = IOHelper::readAs<uint8_t>(file);

    // check running status
    bool incomplete = false;
    auto& message_vec = event->getMessageVec();
    if (cmd < 0x80)
    {
        incomplete = true; // this flag says: do not read to much later
        message_vec.emplace_back(*running_status
        ); // command from previous complete event
        message_vec.emplace_back(cmd);
        cmd = *running_status; // swap
    }
    else
    {
        *running_status = cmd; // current command for this track
        message_vec.emplace_back(cmd);
    }

    auto two_param_events = [&]()
    {
        message_vec.emplace_back(IOHelper::readAs<uint8_t>(file));
        if (!incomplete)
            message_vec.emplace_back(IOHelper::readAs<uint8_t>(file));
    };

    auto one_param_events = [&]()
    {
        if (!incomplete)
            message_vec.emplace_back(IOHelper::readAs<uint8_t>(file));
    };

    auto on_meta_event = [&](auto& meta_event_type)
    {
        auto str_len = IOHelper::readAs<uint8_t>(file);

        if ((meta_event_type == meta_t::SequenceNumber) && (str_len != 2))
        {
            std::cerr << "sequence number event size is not 2 but " << str_len
                      << '\n';
        }

        if ((meta_event_type == meta_t::ChannelPrefix) && (str_len != 1))
        {
            std::cerr << "channel prefix event size is not 1 but" << str_len
                      << '\n';
        }

        if ((meta_event_type == meta_t::OutputCable) && (str_len != 1))
        {
            std::cerr << "output cable event size is not 1 but" << str_len
                      << '\n';
        }

        if ((meta_event_type == meta_t::Tempo) && (str_len != 3))
        {
            std::cerr << "tempo event size is not 3 but" << str_len << '\n';
        }

        if ((meta_event_type == meta_t::SmpteOffset) && (str_len != 5))
        {
            std::cerr << "SMPTE offset event size is not 5 but " << str_len
                      << '\n';
        }

        if (meta_event_type == meta_t::EndOfTrack)
        {

            if (str_len != 0)
            {
                std::cerr << "end of track event size is not 0 but " << str_len
                          << '\n';
            }

            *track_continue = false;
        }

        // read string
        for (int i = 0; i < str_len; i++)
        {
            message_vec.emplace_back(IOHelper::readAs<uint8_t>(file));
        }
    };

    auto meta_or_sysex_events = [&]()
    {
        auto command = msg_t(cmd);
        switch (command)
        {
        case msg_t::Meta: // META events
        {
            auto val = IOHelper::readAs<uint8_t>(file);
            auto meta_event_type = meta_t(val);
            message_vec.emplace_back(val);

            switch (meta_event_type)
            {
            case meta_t::SequenceNumber: // size always 2
            case meta_t::Text:
            case meta_t::Copyright:
            case meta_t::TrackName:
            case meta_t::InstrumentName:
            case meta_t::Lyrics:
            case meta_t::Marker:
            case meta_t::CuePoint:
            case meta_t::ChannelPrefix: // size always 1
            case meta_t::OutputCable: // size always 1
            case meta_t::EndOfTrack: // size always 0
            case meta_t::Tempo: // size always 3
            case meta_t::SmpteOffset: // size always 5
            case meta_t::TimeSignature:
            case meta_t::KeySignature:
                on_meta_event(meta_event_type);
                break;            
            default:
                std::cerr << "unknown meta event 0x" << std::hex << val << '\n';
                break;
            }
            break;
        }
        case msg_t::SysExBegin:
        case msg_t::SysExEnd:
        {
            uint32_t size = IOHelper::getVariableLenQuantity(file);
            for (unsigned int i = 0; i < size; i++)
                message_vec.emplace_back(IOHelper::readAs<uint8_t>(file));
        }
        break;
        
        case msg_t::UNDEFINED_MSG_T:
        default:
            std::cerr << "Undefined command at md::file::readEvent::meta_or_sysex_events: " << command << '\n';
        }
    };
    // control events
    auto msg = msg_t(cmd & 0xf0);

    switch (msg)
    {
    // two parameter events
    case msg_t::NoteOn:
    case msg_t::NoteOff:
    case msg_t::NoteAftertouch:
    case msg_t::ControlChange:
    case msg_t::PitchWheel:
        two_param_events();
        break;

        // one parameter events
    case msg_t::ProgramChange:
    case msg_t::ChannelAftertouch:
        one_param_events();
        break;

    case msg_t::SysExBegin:
        meta_or_sysex_events();
        break;

    default:
        std::cerr << "unknown event " << int(cmd) << '\n';
        break;
    }
}

uint16_t file::getPulsesPerQuarternote() const
{
    return m_ppq;
}

void file::addTrack(const track& track)
{
    m_tracksVec.emplace_back(track);
}

void file::addTrack(track&& track)
{
    m_tracksVec.emplace_back(std::move(track));
}
}
