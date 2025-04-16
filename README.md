# midi_api
Create/edit/play midi files.
This is a fork of cxxmidi (https://github.com/5tan/cxxmidi)

## USAGE:

Add midi_api to your project by copying the github repo:

```bash
git clone https://github.com/AlexeyPeov/midi_api.git
```

Then, add midi_api folder to your CMakeLists
```cmake
include_directories(midi_api/src)
file(GLOB_RECURSE MIDI_API_SOURCES "midi_api/src/*.h" "midi_api/src/*.cpp")

add_executable(your_project_name your_executable.cpp ${MIDI_API_SOURCES})

if(WIN32 OR WIN64)
    target_link_libraries(your_project_name winmm)
endif()

```

Then, you can use "midi_api.h" in your project.

```c++
#include "midi_api.h"
```

## How to build the project if you just want to play:

create /build/debug folder, and cd into it, then run this:

```bash
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER="g++" -S ../../
```

```bash
ninja
```

To build the release version, cd into /build/release


```bash
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER="g++" -S ../../
```

```bash
ninja
```

If you are on Windows, there is a quick .bat script for debug:

```bat
@echo off

if not exist build ( mkdir build )
cd build
if not exist debug ( mkdir debug )
cd debug

@echo off

if not exist build ( mkdir build )
cd build
if not exist debug ( mkdir debug )
cd debug

cmake -G "Ninja" -DCMAKE_C_COMPILER="gcc" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER="g++" -S ../../ && ninja && gdb -iex "set print pretty on" -iex "set print demangle on" -ex "run" -ex "kill" -ex "quit" --quiet  --args midi_api.exe

cd ../../

```


## How to load a midi file:
```c++
#include "midi_api.h"

int main(){
    
    md::file file;
    
    int err = file.load("chopin.mid");
    
    if(err != 0)
        return -1;
    
    // do stuff
    
}
```

## How to play a midi file asynchronously:

```c++

    bool async = true;
    md::player player(async);
    
    // file is a md::file
    player.setFile(std::move(file));
    
    // creates a thread first time you call this function
    player.play();
    
    // stops playback (doesn't join threads)
    player.pause();
    
    // continues
    player.play();
    
    // move playback position (between 0 and 1)
    player.goTo(0.22);

    // if async - will join the playback thread
    player.stop();
    
    // now this will create a thread again
    player.play();
    
    // gives back ownership
    file = player.returnFile();
    
    // returns 0 if succeeded
    int err = file->save("testfile.mid");
    
```

## How to play a midi file synchronously:

```c++

    bool async = false;
    md::player player(async);
    
    // file is a md::file
    player.setFile(std::move(file));
    
    // you can still call goTo, just do it before you call play()
    player.goTo(0.22);

    // basically a while(not_over == true) loop
    player.play();

    file = player.returnFile();
    
    // returns 0 if succeeded
    int err = file->save("testfile.mid");
    
```

##  How to add events to a file:

```c++
    md::file file;

    // timeDiv = number of MIDI ticks per quarter note
    // const uint16_t timeDiv = file.getPulsesPerQuarternote();
    
    md::track track;
    std::vector<md::bar> bars;
    bars.resize(4);

    auto addNote = [](uint8_t note, uint8_t volume) -> std::vector<uint8_t>
    {
        return { md::msg_t::NoteOn, note, volume };
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
    for (auto& bar : bars)
    {

        // this means 5 steps per bar, each step is an eighth note
        bar.setTimeSig(5, 8);

        // since each step is an eighth note, not a quarter note,
        // we need take that into account
        uint32_t step_len = bar.getStepLen(file.getPulsesPerQuarternote());

        // you can access, and set individual events like so:
        // bar[where] += {md::msg_t::NoteOn, c_note, turn_off};
        // or so:
        // bar[where].push_back({md::msg_t::NoteOn, c_note, turn_off});

        // in my case I have addNote method, so I will add events like this:
        // bar[where] += addNote(c_note, turn_on);

        // event at zero
        bar[0] += addNote(c_note, turn_on);

        // last event of a bar
        bar[step_len * bar.stepsPerBeat()] += addNote(c_note, turn_off);

        for (uint32_t j = 1; j < bar.stepsPerBeat(); ++j)
        {

            bar[step_len * j] += addNote(e_note, turn_on);
            bar[step_len * (j + 1)] += addNote(e_note, turn_off);

            bar[step_len * j] += addNote(g_note, turn_on);
            bar[step_len * (j + 1)] += addNote(g_note, turn_off);
        }

        // bar gets converted to series of events,
        // so don't add it to a track unless you're sure you won't edit it
        // later.
        track.addBar(bar);
    }

    // has 2 overloads - const Track&, and Track&&
    file.addTrack(track);

    md::player player;

    // if you want to play a single bar:    
    player.setBars({bars[0]});

    // if you want to play a vector of bars (basically, a track):    
    // function takes const std::vector<bar>&
    player.setBars(bars);

    // adding multiple tracks.
    // function takes const std::vector<std::vector<md::bar>>&
    // player.setBars({ bars, bars });

    player.play();

    // // you can actually convert your bars to a file this way!
    file = player.returnFile();
    file.save("testfile.mid");

```



## WHAT NEEDS TO BE DONE STILL


I've decided to not compile this project into a library, because there is no Linux and MacOS midi playback support yet. You can implement them yourself, by creating ApplePlayer.h and LinuxPlayer.h

There is an interface for them, called MessageInterface
See WindowsPlayer files for more context.
