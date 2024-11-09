#include "raylib.h"
#include <vector>
#include <cmath>

#include "engine/game.hpp"
#include "interface/graphics.hpp"

using std::vector;

void run_game() {
    InitWindow(screenWidth, screenHeight, "Chess Simulator");

    Game g{};
    Graphics graphic{};

    bool piece_pressed = false;
    Square from;
    int x, y;

    SetTargetFPS(60);
    // Main Game Loop
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
                g.Move(from, {x, 7 - y});
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
    run_game();
}


