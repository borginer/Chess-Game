#include "network/game_client.hpp"

namespace wspp = websocketpp;

int main(int argc, char* argv[]) {
    if (argc != 2) { // tbc
        cerr << "invalid arguments" << endl;
        exit(-1);
    }

    PieceColor pov = pov_from_string(argv[1]);
    ChessGame game{};
    GameClient game_client{};

    if (pov == EMPTY_PIECE_COLOR) {
        cout << "no color given" << endl;
        exit(-2);
    }

    // disable raylib logs
    SetTraceLogLevel(LOG_NONE); 

    // game client setup
    game_client.clear_client_logging();
    game_client.set_on_move_recieved([&game](Square from, Square to){
        game.Move(from, to);
    });
    game_client.connect_to_server();
    std::thread client_thread(&GameClient::run, &game_client);
    client_thread.detach();

    // graphic thread
    RayLibInter inter(game, pov);
    inter.SetOnMove([&pov, &game_client](Square from, Square to){
        json j_move = game_prot::do_move_json(from, to, pov, 0);
        game_client.send(j_move.dump());
    });
    inter.run();
}