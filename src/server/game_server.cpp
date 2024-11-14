#include <iostream>

#include "game_server.hpp"

using std::cout, std::cerr, std::endl;

void message_handler(server* serv, connection_hdl hdl,
    msg_ptr msg, ChessGame* game) {
        
    string input = msg->get_payload();
    Square from{}, to{};
    stringstream input_s(input);
    string param;
    vector<string> params;
    cout << "Received: " << input << endl;
    while (getline(input_s, param, ' ')) {
        params.push_back(param);
    }
    from.x = stoi(params[0]);
    from.y = stoi(params[1]);
    to.x = stoi(params[2]);
    to.y = stoi(params[3]);
    
    int res = game->Move(from, to);
    string resp{std::to_string(res)};
    // return the message back
    serv->send(hdl, std::to_string(res), websocketpp::frame::opcode::text);
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

        cout << "Chess game server is running on ws://localhost:" << port << endl;
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