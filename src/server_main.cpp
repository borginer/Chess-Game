#include "network/game_server.hpp"

int main(void) {
    GameServer serv{12345};
    serv.turn_off_logging();
    serv.run();
}