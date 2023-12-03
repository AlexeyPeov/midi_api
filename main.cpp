#include "MidiPlayer.h"

int main() {

    std::unique_ptr<md::file> file = std::make_unique<md::file>();

    file->load("test.mid");

    bool play_async = true;

    md::midi_player player(play_async);

    player.set_file(std::move(file));

    player.play();

    file = player.return_file();

    file->load("chopin.mid");

    player.set_file(std::move(file));

    player.play();

    // do this whenever you're ready to finish the playback
    player.join_threads();

    //file.save_as("chopin_out.mid");

    return 0;
}
