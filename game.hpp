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

struct square {
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

struct move_log {
    int from;
    int to;
    Piece taken;
};

class Game{
public:
    Game();
    // check legality 
    void Move(square, square);
    array<Piece, 64> GetBoard() const {return board;};
private:
    array<Piece, 64> board;
    PieceColor turn;
    int pawn_shadow = -1; // on peasent
    bool queen_castle = false;
    bool king_castle = false;
    //puts all Pieces in place
    void setup();
    // check if the move is legal
    bool checkMove(int from, int to);
    void makeMove(int from, int to);

    bool sameColor(int from, int to){
        return board[from].color == board[to].color;
    };

    bool sameColor(PieceColor p1, PieceColor p2) {
        return p1 == p2;
    }
    // helpers for check move
    vector<int> possibleKnightMoves(int from);
    vector<int> possibleBishopMoves(int from);
    vector<int> possibleRookMoves(int from);
    vector<int> possibleQueenMoves(int from);
    vector<int> possibleKingMoves(int from);
    vector<int> possiblePawnMoves(int from);

    bool pawnMove(int from, int to);
    bool inVec(vector<int> vec, int val);
    void markShadowPawn(int from, int to);
    void removePeasent(int to, PieceColor c);
    void handleOnPeasent(int from, int to);
    void handleCastle(int from, int to);
    // bounds check
    inline bool onBoard(int idx) {return idx > -1 && idx < 64;}
    inline void nextTurn() {turn = turn == white ? black : white;}

};

#endif