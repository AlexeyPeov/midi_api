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

    // time_div = number of MIDI ticks per quarter note
    const uint16_t time_div = file->get_time_div();
    
    // add 1 track to a file (get_tracks() returns a reference to std::vector<md::track>)
    file->get_tracks().emplace_back(time_div);
    
    md::track& track = file->get_tracks()[0];
    
    std::vector<md::bar> bars;
    bars.resize(4);
    
    auto add_note = [](uint8_t note, uint8_t volume) -> std::vector<uint8_t>{
        return {md::msg_t::NoteOn, note, volume};
    };
    
    // notes
    const uint8_t c_note = md::note::C5;
    const uint8_t e_note = md::note::E5;
    const uint8_t g_note = md::note::G5;
    
    // note volumes (if you set volume to zero, a note turns off)
    const uint8_t turn_on = 90;
    const uint8_t turn_off = 0;
    
    // bar sets time signature to 4/4 in a default constructor,
    // meaning 4 steps per beat, each step is a quarter note.
    // However, you can set time signature (4/4, 6/8, 5/4, etc..)
    for(auto& bar : bars){
        
        // this means 5 steps per bar, each step is an eighth note
        bar.set_time_signature(5,8);
        
        // since each step is an eighth note, not a quarter note,
        // we need take that into account
        uint32_t step_len = bar.get_step_len(file->get_time_div());
        
        // you can access, and set individual events like so:
        // bar[where] += {md::msg_t::NoteOn, c_note, turn_off};
        // or so:
        // bar[where].push_back({md::msg_t::NoteOn, c_note, turn_off});
        
        // in my case I have add_note method, so I will add events like this:
        // bar[where] += add_note(c_note, turn_on);
        
        // event at zero
        bar[0] += add_note(c_note, turn_on);
        
        // last event of a bar
        bar[step_len * bar.steps_per_beat()] += add_note(c_note, turn_off);
        
        for(int j = 1; j < bar.steps_per_beat(); ++j){
            
            bar[step_len * j] += add_note(e_note, turn_on);
            bar[step_len * (j+1)] += add_note(e_note, turn_off);
            
            bar[step_len * j] += add_note(g_note, turn_on);
            bar[step_len * (j+1)] += add_note(g_note, turn_off);
            
        }
        
        // bar gets converted to series of events,
        // so don't add it to a track unless you're sure you won't edit it later.
        track.add_bar(bar);
        
    }
    
    md::midi_player player;
    
    // if you want to play a bar:
        // function takes const bar&
        player.set_bars(bars[0]);
        
    // if you want to play a vector of bars (basically, a track):
    std::vector<md::bar>& track1 = bars;
    // function takes const std::vector<bar>&
    player.set_bars(track1);
    
    // adding multiple tracks.
    std::vector<md::bar> track2 = bars;
    // function takes const std::vector<std::vector<md::bar>>&
    player.set_bars({ track1, track2 });
    
    player.play();
    
    // you can actually convert your bars to a file this way!
    file = player.return_file();
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