#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <nlohmann/json.hpp>

#include <functional>

#include "game_prot.hpp"
#include "../engine/chess_game.hpp"

class GameServer {
    websocketpp::server<websocketpp::config::asio> serv;
    ChessGame game;
    int port;
    public:
    GameServer(int port);
    void run();
    void turn_off_logging();
};
