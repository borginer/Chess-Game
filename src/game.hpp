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

struct Square {
    int x = -1, y = -1;
    Square() = default;
    Square(int x, int y){
        this->x = x;
        this->y = y;
    }
    Square(int idx) {
        this->x = idx % 8;
        this->y = idx / 8;
    }
    // get index in board array
    inline int getIdx() {return x + 8 * y;}
    inline bool operator==(Square& other) {
        return this->x == other.x && this->y == other.y;
    }
    inline bool operator!=(Square& other) {
        return *this==other;
    }
    inline Square operator+(Square& other) {
        return Square(this->x + other.x, this->y + other.y);
    }
    inline Square operator-(Square& other) {
        return Square(this->x - other.x, this->y - other.y);
    }
    inline Square operator+(int idx) {
        return Square(this->x + idx % 8, this->y + idx / 8);
    }
    inline Square operator-(int idx) {
        return *this + (-idx);
    }
    inline Square& operator+=(Square& other) {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }
    // checks indexing of input only
    inline bool onBoard(){
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
    void Move(Square, Square);
    array<Piece, 64> GetBoard() const {return board;};
private:
    array<Piece, 64> board;
    int bKingIdx;
    int wKingIdx;
    Square pawn_shadow = {-1, -1}; // on peasent

    array<Piece, 64> copy_board;
    int bKingIdxCopy;
    int wKingIdxCopy;
    Square pawn_shadow_copy;

    bool game_in_progress;
    PieceColor turn;

    bool queen_castle = false;
    bool king_castle = false;
    
    //puts all Pieces in place
    void setup();
    // check if the move is legal
    bool checkMove(Square from, Square to);
    void makeMoveOnCopy(Square from, Square to);

    // find all possible moves for every piece
    vector<int> possibleMoves(Square from);
    // helpers for possibleMoves
    vector<int> possibleKnightMoves(Square from);
    vector<int> possibleBishopMoves(Square from);
    vector<int> possibleRookMoves(Square from);
    vector<int> possibleQueenMoves(Square from);
    vector<int> possibleKingMoves(Square from);
    vector<int> possiblePawnMoves(Square from);

    bool inVec(vector<int> vec, int val);
    // mark Square if pawn double jumped
    void markShadowPawn(Square from, Square to);
    // remove the pawn being capture on peasent
    void removePeasent(Square to, PieceColor c);
    // handle special interactions
    void handleOnPeasent(Square from, Square to);
    void handleCastle(Square from, Square to);
    // checks that the king is under attack, returns true if it isn't
    bool legalPosition();
    // checks if current turn player lost
    bool checkMate();

    // Utils
    inline void updateKingIdxCopy(int idx, PieceColor c) {
        int& king = (c == white) ? wKingIdxCopy : bKingIdxCopy;
        king = idx;
    }
    inline bool onBoard(int idx) {
        return idx > -1 && idx < 64;
    }
    inline void nextTurn() {
        turn = turn == white ? black : white;
    }
    inline Square getSquare(int idx) {
        return {idx % 8, idx / 8};
    }
    inline bool sameColor(int from, int to) {
        return copy_board[from].color == copy_board[to].color;
    }
    inline bool sameColor(Square from, Square to) {
        return sameColor(from.getIdx(), to.getIdx());
    }
    inline bool sameColor(PieceColor p1, PieceColor p2) {
        return p1 == p2;
    }
    inline Piece& getPiece(Square s) {
        return copy_board[s.getIdx()];
    }
    inline Piece& getPiece(int idx) {
        return copy_board[idx];
    }
    
    void commitKingIdx();
    void resetKingIdxCopy();
    void calcKingIdx();
    void printBoard();
    void commitPosition();
    void resetCopyPosition();
    void stopGame();
};

#endif