#include "MidiPlayer.h"

int main() {

    md::file file;
    file.load("chopin.mid");

    md::midi_player player;

    player.set_file(&file);

    player.play();

    //file.save_as("chopin_out.mid");


    return 0;
}
