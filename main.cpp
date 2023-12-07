#include "MidiPlayer.h"



int main() {

    std::unique_ptr<md::file> file = std::make_unique<md::file>();

    file->load("test.mid");

    bool play_async = true;

    md::midi_player player(play_async);

    player.set_file(std::move(file));

    player.play();

    // between 0 and 1
    player.go_to(0.25);

    file = player.return_file();

    file->load("chopin.mid");

    player.set_file(std::move(file));

    player.play();

    // do this whenever you're ready to finish the async playback
    // otherwise just set other files to play with player.set_file()
    player.join_threads();

    //file.save_as("chopin_out.mid");

    return 0;
}
