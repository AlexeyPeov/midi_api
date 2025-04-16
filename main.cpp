#include "midi_api.h"

int main()
{
    md::file file;

    int err = file.load("../../chopin.mid");
    
    if(err != 0)
        return -1;

    md::player player;

    player.setFile(std::move(file));

    player.play();

    return 0;
}
