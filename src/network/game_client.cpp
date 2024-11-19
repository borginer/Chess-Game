#include "game_client.hpp"

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
        cout << "successful move" << endl;
        Square from = { j_move["from.x"], j_move["from.y"] };
        Square to = { j_move["to.x"], j_move["to.y"] };
        cout << from << to << endl;
        g.Move(from, to);
    }
    
}

void on_close(client*, connection_hdl) {
    std::cout << "Connection closed." << std::endl;
}

void run_game(PieceColor perspective) {
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
                
                switch (perspective) {
                    case white:
                    j_move["to.x"] = x;
                    j_move["to.y"] = 7 - y;
                    break;
                    case black:
                    j_move["to.x"] = 7 - x;
                    j_move["to.y"] = y;
                    // cout << from << Square{7 - x, y} << endl;
                    break;
                    default:
                    throw (1);
                }

                c.send(hdl, j_move.dump(), wspp::frame::opcode::text);                
            }
            else {
                piece_pressed = true;
                graphic.SetMarkedSquare({x, y});
                
                switch (perspective) {
                    case white:
                    from = {x, 7 - y};
                    break;
                    case black:
                    from = {7 - x, y};
                    // cout << from << endl;
                    break;
                    default:
                    throw (1);
                }
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            graphic.SetMarkedSquare({-1, -1});
            piece_pressed = false;
        }
        ClearBackground({103, 43, 0});
        graphic.DrawGame(g, perspective);

        EndDrawing();
    }
    CloseWindow();
}

// void run_game() {
//     InitWindow(screenWidth, screenHeight, "Chess Simulator");

//     Graphics graphic{};

//     bool piece_pressed = false;
//     Square from;
//     int x, y;

//     SetTargetFPS(60);
//     // Main ChessGame Loop
//     while (!WindowShouldClose()) {
//         BeginDrawing();

//         if (IsKeyPressed(KEY_U)) {
//             g.UndoMove();
//         }
//         if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
//             Vector2 pos = GetMousePosition();
//             x = std::floor((pos.x - sideBarSize) / figureSize);
//             y = std::floor((pos.y - sideBarSize) / figureSize);
//             if (piece_pressed) {
//                 piece_pressed = false;
//                 graphic.SetMarkedSquare({-1, -1});

//                 json j_move;
//                 j_move["type"] = "move";
//                 j_move["from.x"] = from.x;
//                 j_move["from.y"] = from.y;
//                 j_move["to.x"] = x;
//                 j_move["to.y"] = 7 - y;
//                 c.send(hdl, j_move.dump(), wspp::frame::opcode::text);
//             }
//             else {
//                 piece_pressed = true;
//                 graphic.SetMarkedSquare({x, y});
//                 from = {x, 7 - y};
//             }
//         }
//         if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
//             graphic.SetMarkedSquare({-1, -1});
//             piece_pressed = false;
//         }
//         ClearBackground({103, 43, 0});
//         graphic.DrawGame(g, white);

//         EndDrawing();
//     }
//     CloseWindow();
// }

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "invalid arguments" << endl;
        exit(-1);
    }

    string color = argv[1];
    PieceColor perspective;
    cout << color << endl;
    if (color == "white") { 
        perspective = white; 
    } else if (color == "black") { 
        perspective = black; 
    } else {
        cerr << "no color given" << endl;
        exit(-2);
    }
    cout << perspective << endl;

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

    run_game(perspective);
}