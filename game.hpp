#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <raylib.h>
#include <iostream>

enum player{white = 1, black};
enum types {wKing = 0, wQueen, wRook, wBishop, wKnight, wPawn,
      bKing, bQueen, bRook, bBishop, bKnight, bPawn, TYPES_LAST, EMPTY_TYPE};
enum other {board_txt = TYPES_LAST, OTHER_LAST};

const int boardSize = 800;
const int sideBarSize = 20;
const int screenWidth = boardSize + 2 * sideBarSize;
const int screenHeight = boardSize + 2 * sideBarSize;
const int figureSize = boardSize / 8;

struct square{
    int x = -1, y = -1;
    square() = default;
    square(int x, int y){
        this->x = x;
        this->y = y;
    }
    inline int getIdx() {return x + 8 * y;}
    inline bool operator==(square& other){return this->x == other.x && this->y == other.y;}
};

class Game{
public:
    Game(std::vector<Texture2D>&);
    // check legality 
    void Move(square, square);
    void DrawBoard();
    void SetMarkedSquare(square);
private:
    //put all pieces in place
    void setup();
    // return all legal moves
    std::vector<square> legalMoves(square from);
    std::array<types, 64> board;
    std::vector<Texture2D> textures;
    square marked = {-1, -1};
    player turn;
};

#endif