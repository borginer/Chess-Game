#include "game_client.hpp"

namespace wspp = websocketpp;
using std::vector;
using client = wspp::client<websocketpp::config::asio_client>;
using wspp::connection_hdl;
using msg_ptr = client::message_ptr;
using std::cout, std::cerr, std::endl;
using json = nlohmann::json;

void GameClient::on_open(client* c, connection_hdl _hdl) {
    c_hdl = _hdl;
    std::cout << "Connection opened!" << std::endl;
}

void GameClient::on_message(connection_hdl, msg_ptr msg) {
    string input = msg->get_payload();
    std::cout << "Received: " << input << std::endl;

    json j_move = json::parse(input);
    game_prot::do_move_resp_info info = game_prot::parse_do_move_resp(j_move);

    if (info.res == move_success) {
        cout << "successful move" << endl;
        cout << info.from << info.to << endl;
        if (on_move_recieved) {
            on_move_recieved(info.from, info.to);
        } else {
            cout << "on move received is not set" << endl;
        }
    }
}

void GameClient::on_close(client*, connection_hdl) {
    std::cout << "Connection closed." << std::endl;
}

GameClient::GameClient() {
    game_client.init_asio();

    game_client.set_open_handler(std::bind(
        &GameClient::on_open, this, &game_client,
        std::placeholders::_1));

    game_client.set_message_handler(
        std::bind(&GameClient::on_message, this, 
        std::placeholders::_1, std::placeholders::_2));
        
    game_client.set_close_handler(
        std::bind(&GameClient::on_close, this, &game_client,
        std::placeholders::_1));
}

void GameClient::clear_client_logging() {
    game_client.clear_access_channels(wspp::log::alevel::all);
}

void GameClient::connect_to_server() {
    try {
        wspp::lib::error_code ec;
        client::connection_ptr con = game_client.get_connection("ws://localhost:12345", ec);

        if (ec) {
            std::cerr << "Could not create connection: " << ec.message() << std::endl;
        }

        game_client.connect(con);

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (websocketpp::lib::error_code& e) {
        std::cerr << "Error: " << e.message() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception." << std::endl;
    }
}

void GameClient::run() {
    try {
        game_client.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (websocketpp::lib::error_code& e) {
        std::cerr << "Error: " << e.message() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception." << std::endl;
    }
}

void GameClient::set_on_move_recieved(
    std::function<void(Square from, Square to)> f) {
    
    this->on_move_recieved = f;
}

void GameClient::send(string s) {
    this->game_client.send(this->c_hdl, s, wspp::frame::opcode::text);
}