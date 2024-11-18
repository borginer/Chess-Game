#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <nlohmann/json.hpp>

#include <functional>

#include "../engine/chess_game.hpp"

namespace wspp = websocketpp;

using server = wspp::server<websocketpp::config::asio>;
using msg_ptr = wspp::config::asio::message_type::ptr;
using connection_hdl = wspp::connection_hdl;
void message_handler(server*, connection_hdl, msg_ptr, ChessGame*);

class GameServer {
    server serv;
    ChessGame game;
    int port;
    public:
    GameServer(int port);
    void run();
    void turn_off_logging();
};
