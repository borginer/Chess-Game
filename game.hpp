#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <iostream>
#include <array>

using std::vector;
using std::array;

enum color {white = 0, black, EMPTY_COLOR};

enum type {king = 0, queen, rook, bishop, knight, pawn, EMPTY_TYPE};

enum other {board_txt = 0, EMPTY_OTHER};

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
    inline int getIdx(){return x + 8 * y;}
    inline bool operator==(square& other){
        return this->x == other.x && this->y == other.y;
    }
    // checks indexing of input only
    inline bool validBounds(){
        return !(x < 0 || x > 7 || y < 0 || y > 7);
    }
};

struct piece{
    type t;
    color c;
    piece(){this->t = EMPTY_TYPE; this->c = EMPTY_COLOR;}
    piece(color c, type t){
        this->t = t;
        this->c = c;
    }
    inline bool operator==(piece& other){
        return this->c == other.c && this->t == other.t;
    }
    inline bool operator!=(piece& other){return !operator==(other);}
};

class Game{
public:
    Game();
    // check legality 
    void Move(square, square);
    array<piece, 64> GetBoard(){return board;};
private:
    array<piece, 64> board;
    color turn;

    //put all pieces in place
    void setup();
    // return all legal moves
    vector<square> legalMoves(square from);
    bool checkMove(square from, square to);
    inline bool onBoard(int idx){return idx > -1 && idx < 64;}
    bool checkKnightMove(int from, int to);


};

#endif