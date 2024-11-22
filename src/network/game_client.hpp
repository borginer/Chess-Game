#include "raylib.h"
#include <vector>
#include <cmath>
#include <functional>

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <nlohmann/json.hpp>

#include "../engine/chess_game.hpp"
#include "../interface/graphics.hpp"

#include "game_prot.hpp"
#include "../interface/raylib_inter.hpp"

class GameClient {
    using client = websocketpp::client<websocketpp::config::asio_client>;
    using connection_hdl = websocketpp::connection_hdl;
    using msg_ptr = client::message_ptr;

    client game_client;
    connection_hdl c_hdl;
    std::function<void(Square from, Square to)> on_move_recieved;

    void on_open(client* , connection_hdl);
    void on_message(connection_hdl, msg_ptr);
    void on_close(client*, connection_hdl);
    
    public:
    GameClient();
    void set_on_move_recieved(std::function<void(Square from, Square to)>);
    void clear_client_logging();
    void connect_to_server();
    void run();
    void send(string s);

};