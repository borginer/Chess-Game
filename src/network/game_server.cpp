#include <iostream>

#include "game_server.hpp"

using std::cout, std::cerr, std::endl;
using json = nlohmann::json;

void message_handler(server* serv, connection_hdl hdl,
    msg_ptr msg, ChessGame* game) {
        
    string input = msg->get_payload();
    cout << "Received: " << input << endl;

    json j_move = json::parse(input);

    Square from = { j_move["from.x"], j_move["from.y"] };
    Square to = { j_move["to.x"], j_move["to.y"] };
    
    int res = game->Move(from, to);
    string server_resp{std::to_string(res)};
    // respond with game move code
    json j_resp;
    j_resp["type"] = "move_response";
    j_resp["res"] = res;
    j_resp["from.x"] = from.x;
    j_resp["from.y"] = from.y;
    j_resp["to.x"] = to.x;
    j_resp["to.y"] = to.y;
    serv->send(hdl, j_resp.dump(), wspp::frame::opcode::text);
}

GameServer::GameServer(int port) : serv{}, game{} {
    this->port = port;
}

void GameServer::turn_off_logging() {
  serv.clear_access_channels(wspp::log::alevel::all);
  serv.clear_error_channels(wspp::log::elevel::all);
}

void GameServer::run() {
    try {
        serv.set_message_handler([this](connection_hdl hdl, msg_ptr msg) {
            message_handler(&this->serv, hdl, msg, &this->game);
        });

        serv.init_asio();
        serv.listen(port);
        serv.start_accept();

        cout << "Chess game server is running on ws://localhost:"
            << port << endl;
        serv.run();
    } catch (wspp::exception const & e) {
        cerr << "WebSocket++ Error: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown Error" << endl;
    }
}
int main() {
    GameServer serv{12345};
    serv.turn_off_logging();
    serv.run();

    return 0;
}