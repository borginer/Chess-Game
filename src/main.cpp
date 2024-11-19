#include "raylib.h"
#include <vector>
#include <cmath>

#include "engine/chess_game.hpp"
#include "interface/graphics.hpp"

using std::vector;

void run_game(PieceColor perspective) {
    InitWindow(screenWidth, screenHeight, "Chess Simulator");

    ChessGame g{};
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

                switch (perspective) {
                    case white:
                    g.Move(from, {x, 7 - y});
                    break;
                    case black:
                    g.Move(from, {7 - x, y});
                    // cout << from << Square{7 - x, y} << endl;
                    break;
                    default:
                    throw (1);
                }
                
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
    run_game(perspective);
}