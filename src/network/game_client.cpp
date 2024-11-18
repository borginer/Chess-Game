#include "raylib.h"
#include <vector>
#include <cmath>
#include <functional>

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <nlohmann/json.hpp>

#include "../engine/chess_game.hpp"
#include "../interface/graphics.hpp"

namespace wspp = websocketpp;
using std::vector;
using client = wspp::client<websocketpp::config::asio_client>;
using wspp::connection_hdl;
using msg_ptr = client::message_ptr;
using std::cout, std::cerr, std::endl;
using json = nlohmann::json;

static client c;
static ChessGame g{};
static connection_hdl hdl;

void on_open(client* c, connection_hdl _hdl) {
    hdl = _hdl;
    std::cout << "Connection opened!" << std::endl;
}

void on_message(connection_hdl, msg_ptr msg) {
    string input = msg->get_payload();
    std::cout << "Received: " << input << std::endl;

    json j_move = json::parse(input);

    if (j_move["res"] == move_success) {
        Square from = { j_move["from.x"], j_move["from.y"] };
        Square to = { j_move["to.x"], j_move["to.y"] };
        g.Move(from, to);
    }
    
}

void on_close(client*, connection_hdl) {
    std::cout << "Connection closed." << std::endl;
}


void run_game() {
    InitWindow(screenWidth, screenHeight, "Chess Simulator");

    Graphics graphic{};

    bool piece_pressed = false;
    Square from;
    int x, y;

    SetTargetFPS(60);
    // Main ChessGame Loop
    while (!WindowShouldClose()) {
        BeginDrawing();

        if (IsKeyPressed(KEY_U)) {
            g.UndoMove();
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 pos = GetMousePosition();
            x = std::floor((pos.x - sideBarSize) / figureSize);
            y = std::floor((pos.y - sideBarSize) / figureSize);
            if (piece_pressed) {
                piece_pressed = false;
                graphic.SetMarkedSquare({-1, -1});

                json j_move;
                j_move["type"] = "move";
                j_move["from.x"] = from.x;
                j_move["from.y"] = from.y;
                j_move["to.x"] = x;
                j_move["to.y"] = 7 - y;
                c.send(hdl, j_move.dump(), wspp::frame::opcode::text);
            }
            else {
                piece_pressed = true;
                graphic.SetMarkedSquare({x, y});
                from = {x, 7 - y};
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            graphic.SetMarkedSquare({-1, -1});
            piece_pressed = false;
        }
        ClearBackground({103, 43, 0});
        graphic.DrawGame(g);

        EndDrawing();
    }
    CloseWindow();
}

int main() {
    // suppress raylib logs
    SetTraceLogLevel(LOG_NONE);
    try {
        c.init_asio();

        c.set_open_handler(std::bind(&on_open, &c, std::placeholders::_1));
        c.set_message_handler(std::bind(&on_message, std::placeholders::_1, std::placeholders::_2));
        c.set_close_handler(std::bind(&on_close, &c, std::placeholders::_1));
        // clear logging
        c.clear_access_channels(wspp::log::alevel::all);

        // Create a connection
        wspp::lib::error_code ec;
        client::connection_ptr con = c.get_connection("ws://localhost:12345", ec);

        if (ec) {
            std::cerr << "Could not create connection: " << ec.message() << std::endl;
            return 1;
        }

        // Queue connection and run the I/O service
        c.connect(con);

        std::thread client_thread(&client::run, &c);
        client_thread.detach();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (websocketpp::lib::error_code& e) {
        std::cerr << "Error: " << e.message() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception." << std::endl;
    }

    run_game();
}