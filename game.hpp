#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <raylib.h>
#include <iostream>

enum player{white = 1, black};
enum types {wKing = 0, wQueen, wRook, wBishop, wKnight, wPawn,
      bKing, bQueen, bRook, bBishop, bKnight, bPawn, board_txt, TYPES_LAST, EMPTY_TYPE};

const int boardSize = 800;
const int sideBarSize = 20;
const int screenWidth = boardSize + 2 * sideBarSize;
const int screenHeight = boardSize + 2 * sideBarSize;
const int figureSize = boardSize / 8;

class Game{
public:
    Game(std::vector<Texture2D>&);
    // check legality 
    void Move(std::pair<int,int>, std::pair<int,int>);
    void DrawBoard();
private:
    void setup();
    std::array<types, 64> board;
    std::vector<Texture2D> textures;
    player turn;
};

#endif