#include "MidiPlayer.h"

int main() {



    md::file file;
    file.load("chopin.mid");

    md::midi_player player;

    player.set_file(&file);

    player.go_to(0.72);

    player.play();

    //file.save_as("chopin_out.mid");


    return 0;
}
