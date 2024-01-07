# midi_api
Create/edit/play midi files

## USAGE:

Add midi_api to your project by copying the github repo:

```bash
git clone https://github.com/AlexeyPeov/midi_api.git
```

Then, add midi_api folder to your CMakeLists
```cmake
include_directories(midi_api/Headers midi_api/Source)
file(GLOB_RECURSE MIDI_API_SOURCES "midi_api/Headers/*.h" "midi_api/Source/*.cpp")

add_executable(your_project_name your_executable.cpp ${MIDI_API_SOURCES})

if(WIN32 OR WIN64)
    target_link_libraries(your_project_name winmm)
endif()

```

Then, you can use "midi_api.h" in your project.

```c++
#include "midi_api.h"
```

## How to load a midi file:
```c++
#include "midi_api.h"

int main(){
    
    std::unique_ptr<md::file> file = std::make_unique<md::file>();
    
    int err = file->load("your_midi_file.mid");
    
    if(err != 0)
        return -1;
    
    // do stuff
    
}
```

## How to play a midi file asynchronously:

```c++

    bool async = true;
    md::midi_player player(async);
    
    // file is a std::unique_ptr<md::file>
    player.set_file(std::move(file));
    
    // creates a thread first time you call this function
    player.play();
    
    // stops playback (doesn't join threads)
    player.stop();
    
    // continues
    player.play();
    
    // move playback position (between 0 and 1)
    player.go_to(0.22);

    // self-explanatory
    player.join_threads();
    
    // now this will create a thread again
    player.play();
    
    // gives back ownership
    file = player.return_file();
    
    // returns 0 if succeeded
    int err = file->save_as("testfile.mid");
    
```

## How to play a midi file synchronously:

```c++

    bool async = false;
    md::midi_player player(async);
    
    // file is a std::unique_ptr<md::file>
    player.set_file(std::move(file));
    
    // you can still call go_to, just do it before you call play()
    player.go_to(0.22);

    // basically a while(not_over == true) loop
    player.play();

    file = player.return_file();
    
    // returns 0 if succeeded
    int err = file->save_as("testfile.mid");
    
```

##  How to add events to a file:

```c++
    std::unique_ptr<md::file> file = std::make_unique<md::file>();
    
    // qnl = quarter note length (in milliseconds)
    const uint16_t qnl = file->get_qnl();

    // add 1 track to a file (get_tracks() returns a reference to std::vector<md::track>) 
    file->get_tracks().emplace_back(qnl);

    md::track& track = tracks[0];

    
    md::bar bar();
    
    // default is 4/4, meaning 4 quarter notes per beat / 4 beats per bar.
    // However, you can set this to whatever time sig you want (4/4, 6/8, 5/4, etc..)
    bar.set_time_signature(3,4);
    
    const auto note_on = (uint8_t)md::MidiMessageType::NoteOn;
    const auto c_note = (uint8_t)md::note::C5;
    const auto e_note = (uint8_t)md::note::E5;
    const auto g_note = (uint8_t)md::note::G5;
    
    // note volume
    const uint8_t turn_on = 90;
    const uint8_t turn_off = 0;
    
    // here is an example with 3/4 time signature
    // there are 4 beats and 3 quarter notes per each beat
    // track's delta_time (or quarter_note_length/qnl) is equal to file's qnl
    
    auto dt = file->get_qnl();
    auto beat_len = dt * bar.qn_per_beat();
    
    // you can access, and set individual events like so:
    // bar[where] += {note_on, c_note, turn_off};
    // or so:
    // bar[where].push_back({note_on, c_note, turn_off});
    
    for(int i = 0; i < bar.beats_per_bar(); ++i){

        bar[i * beat_len] += {note_on, c_note, turn_on};
        bar[(i+1) * beat_len] += {note_on, c_note, turn_off};
    
        bar[(i * beat_len) + dt] += {note_on, e_note, turn_on};
        bar[(i * beat_len) + dt*2] += {note_on, e_note, turn_off};
    
        bar[(i * beat_len) + dt] += {note_on, g_note, turn_on};
        bar[(i * beat_len) + dt*2] += {note_on, g_note, turn_off};
        
        bar[(i * beat_len) + dt*2] += {note_on, e_note, turn_on};
        bar[(i * beat_len) + dt*3] += {note_on, e_note, turn_off};
        
        bar[(i * beat_len) + dt*2] += {note_on, g_note, turn_on};
        bar[(i * beat_len) + dt*3] += {note_on, g_note, turn_off};
    
    }
    
    // bar gets converted to series of events,
    // so don't add it to a track unless you're sure you won't edit it later.
    track.add_bar(bar);
    
    file->save_as("testfile.mid");

```



## WHAT NEEDS TO BE DONE STILL


I've decided to not compile this project into a library, because there is no Linux and MacOS midi playback support yet. You can implement them yourself, by creating ApplePlayer.h and LinuxPlayer.h

There is an interface for them, called MessageInterface:
```c++
namespace md{

    class MessageInterface {
    public:

        MessageInterface() = default;

        virtual ~MessageInterface() = default;

        MessageInterface(const MessageInterface &) = delete;

        MessageInterface &operator=(const MessageInterface &) = delete;

        virtual void open_port(uint32_t port_num) = 0;

        virtual void close_port() = 0;

        virtual void open_virtual_port(const std::string &port_name) = 0;

        virtual size_t get_port_count() = 0;

        virtual std::string get_port_name(uint32_t port_num) = 0;

        virtual void send_message(const std::vector<uint8_t> *msg) = 0;

        void reset();

    protected:
        virtual void init() = 0;
        bool m_connected = false;
    };

}
```

See WindowsPlayer files for more context.