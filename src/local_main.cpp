#include "raylib.h"
#include <vector>
#include <cmath>

#include "engine/chess_game.hpp"
#include "interface/graphics.hpp"
#include "interface/raylib_inter.hpp"

using std::vector;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "invalid arguments" << endl;
        exit(-1);
    }

    string color = argv[1];
    PieceColor pov;
    cout << color << endl;
    if (color == "white") { 
        pov = white; 
    } else if (color == "black") { 
        pov = black; 
    } else {
        cerr << "no color given" << endl;
        exit(-2);
    }
    cout << pov << endl;

    ChessGame game{};
    RayLibInter inter(game, pov);
    inter.SetOnMove([&game](Square from, Square to){
        game.Move(from, to);
    });
    inter.run();
}