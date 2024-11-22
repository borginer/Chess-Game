#include "raylib_inter.hpp"

RayLibInter::RayLibInter(ChessGame& game, PieceColor pov) {
    cout << "building interface" << endl;
    InitWindow(screenWidth, screenHeight, "Chess Simulator");

    this->piece_pressed = false;
    this->idx_x = 0; this->idx_y = 0;
    this->pov = pov;
    this->game = &game;
    this->graphic = new Graphics{};

    this->on_move = [](Square a, Square b){};
    this->on_undo = []{};
}

void RayLibInter::SetOnMove(std::function<void(Square, Square)> f) {
    this->on_move = f;
}

void RayLibInter::SetOnUndo(std::function<void()> f) {
    this->on_undo = f;
}

Square get_square_from_pos(int x, int y, PieceColor pov) {
    Square ret;
    
    if (pov == white) {
        ret = { x, 7 - y };
    } else if (pov == black) {
        ret = { 7 - x, y };
    } else {
        throw(1);
    }

    return ret;
}

void RayLibInter::run() {
    SetTargetFPS(60);
    // Main ChessGame Loop
    while (!WindowShouldClose()) {
        BeginDrawing();

        // if (IsKeyPressed(KEY_U)) {
        //     game->UndoMove();
        // }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 pos = GetMousePosition();
            idx_x = std::floor((pos.x - sideBarSize) / figureSize);
            idx_y = std::floor((pos.y - sideBarSize) / figureSize);

            if (piece_pressed) {
                piece_pressed = false;
                graphic->SetMarkedSquare({-1, -1});

                to = get_square_from_pos(idx_x, idx_y, pov);

                on_move(from, to);                
            }
            else {
                piece_pressed = true;
                graphic->SetMarkedSquare({idx_x, idx_y});
                
                from = get_square_from_pos(idx_x, idx_y, pov);
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            graphic->SetMarkedSquare({-1, -1});
            piece_pressed = false;
        }
        ClearBackground({103, 43, 0});
        graphic->DrawGame(*game, pov);

        EndDrawing();
    }
    CloseWindow();
}

PieceColor pov_from_string(string color) {
    PieceColor pov;

    if (color == "white") { 
        pov = white; 
    } else if (color == "black") { 
        pov = black; 
    } else {
        pov = EMPTY_PIECE_COLOR;
    }

    return pov;
}
