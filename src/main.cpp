#include "Game.h"

#include <cstdlib>
#include <ctime>

int main(int /*argc*/, char* /*argv*/[]) {
    srand(static_cast<unsigned int>(time(nullptr)));

    Game game;
    if (!game.init()) {
        return 1;
    }
    game.run();
    return 0;
}
