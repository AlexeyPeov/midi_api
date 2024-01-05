#include "file.h"

namespace md {

    uint32_t file::write(
            std::ofstream &file,
            const uint8_t *c,
            std::streamsize size
    ) {
        file.write(reinterpret_cast<const char *>(c), size);
        return size;
    }

    file::file() {
        m_quarter_note_len = 500;
    }

    file::file(const char *path) {
        m_quarter_note_len = 500;
        load(path);
    }

    int file::save_as(const char *path) const {
        std::ofstream out_file(path, std::ios_base::binary);

        if (!out_file.good()) {
            std::cerr << "file::save_as could not open file " << path << '\n';
            return -1;
        }

        save_header_chunk(out_file);

        for (const auto &track: m_tracks_vec) {
            save_track_chunk(out_file, track);
        }

        out_file.close();
        return 0;
    }

    const std::vector<track> &file::get_tracks() const {
        return m_tracks_vec;
    }

    std::vector<track> &file::get_tracks() {
        return m_tracks_vec;
    }

    void file::save_track_chunk(
            std::ofstream &output_file,
            const track &track
    ) {

        IOHelper::write_as<uint32_t>(output_file, 0x4D54726B);  // "MTrk"

        auto size_pos = output_file.tellp();

        IOHelper::write_as<uint32_t>(output_file, 0);

        uint32_t chunk_size = 0;
        uint8_t last_cmd = 0;

        for(const auto &e: track.get_events()){
            chunk_size += save_event(output_file, e, &last_cmd);
        }


        output_file.seekp(size_pos);

        IOHelper::write_as<uint32_t>(output_file, chunk_size);

        output_file.seekp(0, std::ios_base::end);
    }

    uint32_t file::save_event(
            std::ofstream &output_file,
            const event &event,
            uint8_t *last_cmd
    ) {


        uint32_t r = IOHelper::save_variable_len_quantity(
                output_file,
                event.dt()
        );

        if (event.empty()) {
            std::cout << "EVENT EMPTY AT FILE::SAVE_EVENT\n";
            return r;
        }


        int skip_data_bytes;
        if (event.is_sysex()) {
            auto first = event.get_message_vec().at(0);
            r += write(output_file, &first);

            uint8_t data_size = static_cast<uint8_t>(event.size()) - 1;
            r += IOHelper::save_variable_len_quantity(output_file, data_size);

            skip_data_bytes = 1;
            *last_cmd = 0;

        } else if (event.is_meta()) {
            auto data = event.get_message_vec().data();
            r += write(output_file, data, 2);  // byte 0 and 1

            uint8_t data_size = static_cast<uint8_t>(event.size()) - 2;
            r += write(output_file, &data_size);  // save length

            skip_data_bytes = 2;
            *last_cmd = 0;
        } else {
            const uint8_t nowCmd = event.get_message_vec()[0];
            skip_data_bytes = (*last_cmd == nowCmd);
            *last_cmd = nowCmd;  // save current command if we use running status
        }

        r += write(
                output_file,
                event.get_message_vec().data() + skip_data_bytes,
                event.get_message_vec().size() - skip_data_bytes
        );
        return r;
    }

    void file::save_header_chunk(std::ofstream &output_file) const {

        IOHelper::write_as<uint32_t>(output_file, 0x4D546864);  // "MThd"

        // save header size
        IOHelper::write_as<uint32_t>(output_file, 6);

        // save file type
        auto tr_sz = m_tracks_vec.size();
        uint16_t fileType = (tr_sz > 1) ? 1 : 0;
        IOHelper::write_as<uint16_t>(output_file, fileType);

        // save tracks number
        IOHelper::write_as<uint16_t>(output_file, static_cast<uint16_t>(tr_sz));

        // save time division
        IOHelper::write_as<uint16_t>(output_file, m_quarter_note_len);
    }


    int file::load(const char *path) {
        std::ifstream file(path, std::ifstream::in | std::ifstream::binary);

        if (!file.is_open()) {
            std::cerr << "mapi::file::load couldn't open: " << path << '\n';
            return -1;
        }

        m_tracks_vec.clear();

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

        while (file.good() && (file_len - file.tellg())) {
            auto chunk_id = IOHelper::read_as<uint32_t>(file);

            switch (chunk_id) {
                case msg_type_header:
                    read_header_chunk(file);
                    headers++;
                    break;

                case msg_type_track:
                    read_track_chunk(file);
                    break;

                default:
                    std::cerr << "mapi::file::load ignoring unknown chunk: 0x"
                              << std::hex
                              << static_cast<int>(chunk_id) << '\n';
                    read_unknown_chunk(file);
                    break;
            }

            if (!i++ && !headers) {
                std::cerr << "mapi::file::load no header chunk "
                             "(probably not a MIDI file)"
                          << '\n';
                *this = md::file();
                return -1;
            }
        }

        return 0;
    }

    void file::read_header_chunk(std::ifstream &file) {
        // read and check header size
        if (IOHelper::read_as<uint32_t>(file) != 6) {
            std::cerr << "unsupported MIDI file type" << '\n';
        }

        // read file type
        auto type = IOHelper::read_as<uint16_t>(file);

        // check file type
        if ((type != 0) && (type != 1))
            std::cerr << "unsupported MIDI file type: " << type << '\n';
        // type 0: single track
        // type 1: multi-track

        // read tracks number
        auto tracks_num = IOHelper::read_as<uint16_t>(file);

        m_tracks_vec.reserve(tracks_num);

        // read time division
        m_quarter_note_len = IOHelper::read_as<uint16_t>(file);
        // check time division
        if (m_quarter_note_len & 0x8000)
            std::cerr << "unsupported MIDI file time division" << '\n';
    }

    void file::read_unknown_chunk(std::ifstream &file) {
        auto chunk_size = IOHelper::read_as<uint32_t>(file);
        // skip chunk data
        file.seekg(chunk_size, std::ifstream::cur);
    }

    void file::read_track_chunk(std::ifstream &file) {
        m_tracks_vec.emplace_back(m_quarter_note_len);
        track& track = m_tracks_vec.back();

        auto chunk_size = IOHelper::read_as<uint32_t>(file);

        uint8_t running_status = 0;
        auto begin = file.tellg();
        bool track_continue = true;


        while (track_continue) {
            event event;

            uint32_t dt = IOHelper::get_variable_len_quantity(file);

            event.set_dt(dt);

            read_event(file, &event, &track_continue, &running_status);

            track.get_events().emplace_back(event);
        }

        if (chunk_size != (file.tellg() - begin))
            std::cerr << "track data and track chunk size mismatch" << '\n';
    }


    void file::read_event(
            std::ifstream &file,
            event *event,
            bool *track_continue,
            uint8_t *running_status
    ) {
        auto cmd = IOHelper::read_as<uint8_t>(file);

        // check running status
        bool incomplete = false;
        auto &message_vec = event->get_message_vec();
        if (cmd < 0x80) {
            incomplete = true;  // this flag says: do not read to much later
            message_vec.emplace_back(
                    *running_status);  // command from previous complete event
            message_vec.emplace_back(cmd);
            cmd = *running_status;  // swap
        } else {
            *running_status = cmd;  // current command for this track
            message_vec.emplace_back(cmd);
        }

        auto two_param_events = [&]() {
            message_vec.emplace_back(IOHelper::read_as<uint8_t>(file));
            if (!incomplete)
                message_vec.emplace_back(IOHelper::read_as<uint8_t>(file));
        };

        auto one_param_events = [&]() {
            if (!incomplete)
                message_vec.emplace_back(IOHelper::read_as<uint8_t>(file));
        };

        auto on_meta_event = [&](auto &meta_event_type) {
            auto str_len = IOHelper::read_as<uint8_t>(file);

            if ((meta_event_type == MidiMetaType::SequenceNumber) &&
                (str_len != 2)) {
                std::cerr << "sequence number event size is not 2 but "
                          << str_len << '\n';
            }

            if ((meta_event_type == MidiMetaType::ChannelPrefix) &&
                (str_len != 1)) {
                std::cerr << "channel prefix event size is not 1 but" << str_len
                          << '\n';
            }


            if ((meta_event_type == MidiMetaType::OutputCable) &&
                (str_len != 1)) {
                std::cerr << "output cable event size is not 1 but" << str_len
                          << '\n';
            }

            if ((meta_event_type == MidiMetaType::Tempo) && (str_len != 3)) {
                std::cerr << "tempo event size is not 3 but" << str_len << '\n';
            }


            if ((meta_event_type == MidiMetaType::SmpteOffset) &&
                (str_len != 5)) {
                std::cerr << "SMPTE offset event size is not 5 but " << str_len
                          << '\n';
            }

            if (meta_event_type == MidiMetaType::EndOfTrack) {

                if (str_len != 0) {
                    std::cerr << "end of track event size is not 0 but "
                              << str_len << '\n';
                }

                *track_continue = false;
            }

            // read string
            for (int i = 0; i < str_len; i++) {
                message_vec.emplace_back(IOHelper::read_as<uint8_t>(file));
            }

        };

        auto meta_or_sysex_events = [&]() {
            auto command = MidiMessageType(cmd);
            switch (command) {
                case MidiMessageType::Meta:  // META events
                {
                    auto val = IOHelper::read_as<uint8_t>(file);
                    auto meta_event_type = MidiMetaType(val);
                    message_vec.emplace_back(val);

                    switch (meta_event_type) {
                        case MidiMetaType::SequenceNumber:  // size always 2
                        case MidiMetaType::Text:
                        case MidiMetaType::Copyright:
                        case MidiMetaType::TrackName:
                        case MidiMetaType::InstrumentName:
                        case MidiMetaType::Lyrics:
                        case MidiMetaType::Marker:
                        case MidiMetaType::CuePoint:
                        case MidiMetaType::ChannelPrefix:  // size always 1
                        case MidiMetaType::OutputCable:    // size always 1
                        case MidiMetaType::EndOfTrack:     // size always 0
                        case MidiMetaType::Tempo:          // size always 3
                        case MidiMetaType::SmpteOffset:    // size always 5
                        case MidiMetaType::TimeSignature:
                        case MidiMetaType::KeySignature:
                            on_meta_event(meta_event_type);
                            break;
                        default:
                            std::cerr << "CxxMidi: unknown meta event 0x"
                                      << std::hex << val << '\n';
                            break;
                    }
                    break;
                }
                case MidiMessageType::SysExBegin:
                case MidiMessageType::SysExEnd: {
                    uint32_t size = IOHelper::get_variable_len_quantity(file);
                    for (unsigned int i = 0; i < size; i++)
                        message_vec.emplace_back(
                                IOHelper::read_as<uint8_t>(file));
                }
                    break;
            }
        };
        // control events
        auto msg = MidiMessageType(cmd & 0xf0);

        switch (msg) {
            // two parameter events
            case MidiMessageType::NoteOn:
            case MidiMessageType::NoteOff:
            case MidiMessageType::NoteAftertouch:
            case MidiMessageType::ControlChange:
            case MidiMessageType::PitchWheel:
                two_param_events();
                break;

                // one parameter events
            case MidiMessageType::ProgramChange:
            case MidiMessageType::ChannelAftertouch:
                one_param_events();
                break;

            case MidiMessageType::SysExBegin:
                meta_or_sysex_events();
                break;

            default:
                std::cerr << "unknown event " << int(cmd) << '\n';
                break;

        }
    }

    uint16_t file::get_quarter_note_len() const {
        return m_quarter_note_len;
    }

    uint16_t file::get_qnl() const {
        return m_quarter_note_len;
    }

}

