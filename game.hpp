#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <iostream>
#include <array>
#include <string>
#include <chrono>

using namespace std;

const int boardSize = 800;
const int sideBarSize = 20;
const int screenWidth = boardSize + 2 * sideBarSize;
const int screenHeight = boardSize + 2 * sideBarSize;
const int figureSize = boardSize / 8;

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
    array<Piece, 64> copy_board;
    bool game_in_progress;
    PieceColor turn;
    int pawn_shadow = -1; // on peasent
    bool queen_castle = false;
    bool king_castle = false;
    int bKingIdx;
    int wKingIdx;
    //puts all Pieces in place
    void setup();
    // check if the move is legal
    bool checkMove(int from, int to);
    void makeMoveOnCopy(int from, int to);

    // find all possible moves for every piece
    vector<int> possibleMoves(int from);
    // helpers for possibleMoves
    vector<int> possibleKnightMoves(int from);
    vector<int> possibleBishopMoves(int from);
    vector<int> possibleRookMoves(int from);
    vector<int> possibleQueenMoves(int from);
    vector<int> possibleKingMoves(int from);
    vector<int> possiblePawnMoves(int from);

    bool inVec(vector<int> vec, int val);
    // mark square if pawn double jumped
    void markShadowPawn(int from, int to);
    // remove the pawn being capture on peasent
    void removePeasent(int to, PieceColor c);
    // handle special interactions
    void handleOnPeasent(int from, int to);
    void handleCastle(int from, int to);
    // checks that the king is under attack, returns true if it isn't
    bool legalPosition();
    // checks if current turn player lost
    bool checkMate();
    
    inline void updateKingIdx(int idx, PieceColor c) {
        int& king = (c == white) ? wKingIdx : bKingIdx;
        king = idx;
    }
    inline bool onBoard(int idx) {return idx > -1 && idx < 64;}
    inline void nextTurn() {turn = turn == white ? black : white;}
    inline square getSquare(int idx) {return {idx % 8, idx / 8};}
    inline bool sameColor(int from, int to){
        return copy_board[from].color == copy_board[to].color;
    };
    inline bool sameColor(PieceColor p1, PieceColor p2) {
        return p1 == p2;
    }

    // helper for diagonal moves
    bool onEdge(int idx);
    void printBoard();
};

#endif