#include <functional>

#include "raylib.h"

#include "../engine/chess_game.hpp"
#include "../interface/graphics.hpp"

class RayLibInter {
    bool piece_pressed;
    Square from; Square to;
    int idx_x, idx_y;
    PieceColor pov;
    ChessGame* game;
    Graphics* graphic;

    std::function<void(Square, Square)> on_move;
    std::function<void()> on_undo;
    public:
    RayLibInter(ChessGame& game, PieceColor pov);
    void SetOnMove(std::function<void(Square, Square)>);
    void SetOnUndo(std::function<void()>);
    
    void run();
};

PieceColor pov_from_string(string color);