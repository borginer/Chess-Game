#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <iostream>
#include <array>

using namespace std;

enum PieceColor {
    white = 0,
    black,
    EMPTY_PIECE_COLOR
};

enum Type {
    king = 0,
    queen,
    rook,
    bishop,
    knight,
    pawn,
    EMPTY_TYPE
};

enum Other {
    board_txt = 0,
    EMPTY_OTHER
};

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
    // get index in board array
    inline int getIdx(){return x + 8 * y;}
    inline bool operator==(square& other){
        return this->x == other.x && this->y == other.y;
    }
    // checks indexing of input only
    inline bool validBounds(){
        return !(x < 0 || x > 7 || y < 0 || y > 7);
    }
};

struct Piece{
    Type type;
    PieceColor color;
    bool moved;
    Piece(){this->type = EMPTY_TYPE; this->color = EMPTY_PIECE_COLOR;}
    Piece(PieceColor c, Type t){
        this->type = t;
        this->color = c;
        this->moved = false;
    }
    inline bool operator==(Piece& other){
        return this->color == other.color && this->type == other.type;
    }
    inline bool operator!=(Piece& other){return !operator==(other);}
};

class Game{
public:
    Game();
    // check legality 
    void Move(square, square);
    array<Piece, 64> GetBoard() const {return board;};
private:
    array<Piece, 64> board;
    //array<set<const Piece*>, 64> attack;
    PieceColor turn;
    //puts all Pieces in place
    void setup();
    // returns all legal moves
    vector<square> legalMoves(square from);
    // check if the move is legal
    bool checkMove(square from, square to);

    bool sameColor(int from, int to){
        return board[from].color == board[to].color;
    };

    bool sameColor(PieceColor p1, PieceColor p2) {
        return p1 == p2;
    }
    // helpers for check move
    bool checkKnightMove(int from, int to);
    bool checkBishopMove(int from, int to);
    bool checkRookMove(int from, int to);
    bool checkQueenMove(int from, int to);
    bool checkKingMove(int from, int to);
    bool checkPawnMove(int from, int to);
    // bounds check
    inline bool onBoard(int idx){return idx > -1 && idx < 64;}

};

#endif