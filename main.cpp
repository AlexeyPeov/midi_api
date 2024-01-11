#include "midi_api.h"


int main() {
    auto file = std::make_unique<md::file>();

    file->load("chopin.mid");

    md::midi_player player;

    player.set_file(std::move(file));

    player.play();

    return 0;
}
