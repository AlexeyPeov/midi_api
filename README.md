# midi_api
Create/edit/play midi files

WORK IN PROGRESS

Usage:
#include "MidiPlayer.h"

int main() {

    std::unique_ptr<md::file> file = std::make_unique<md::file>();

    file->load("test.mid");               // whatever .mid file you like

    bool play_async = true; // play on a separate thread

    md::midi_player player(play_async);

    player.set_file(std::move(file));     // give ownership of a file to the player
    
    player.play();
    player.go_to(0.25); // you can move the playback position within 0 and 1

    file = player.return_file();         // return ownership

    // do this when you're ready to finish the playback
    // you can always call play() on a player again whenever you want
    player.join_threads();

    //file.save_as("chopin_out.mid");

    return 0;
}
