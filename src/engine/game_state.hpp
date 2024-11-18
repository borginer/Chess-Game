#pragma once 

#define BOARD_SIZE 64

#include <iostream>
#include <array>
#include <vector>

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

struct Square {
    short x = -1, y = -1;
    Square() = default;
    Square(int x, int y) {
        this-> x = (short)x;
        this-> y = (short)y;
    }
    Square(short idx) {
        this->x = idx % 8;
        this->y = idx / 8;
    }
    
    inline bool operator==(Square &other) {
        return this->x == other.x && this->y == other.y;
    }
    inline bool operator!=(Square &other) {
        return !(*this == other);
    }
    inline Square operator+(Square &other) {
        return Square(this->x + other.x, this->y + other.y);
    }
    inline Square operator-(Square &other) {
        return Square(this->x - other.x, this->y - other.y);
    }
    inline Square operator+(short idx) {
        return Square(this->x + idx % 8, this->y + idx / 8);
    }
    inline Square operator-(short idx) {
        return *this + (-idx);
    }
    inline Square operator*(int num) {
        return { this->x * num, this->y * num };
    }
    inline Square &operator+=(Square &other) {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }
    friend ostream& operator<<(ostream& os, const Square& s) {
        os << "Square: (" << s.x << "," << s.y << ")" << endl;
        return os;
    }
    // checks wether square is on the chess boards
    inline bool onBoard() {
        return !(x < 0 || x > 7 || y < 0 || y > 7);
    }
    // get index in board array
    inline short getIdx() { return x + 8 * y; }
    private:
};

struct MovePair {
    Square from;
    Square to;
};

struct Piece {
    Type type;
    PieceColor color;
    bool moved;

    Piece();
    Piece(PieceColor c, Type t);
    Piece(PieceColor c, Type t, bool moved);
    Piece(const Piece &other);

    inline bool operator==(Piece &other) {
        return this->color == other.color && this->type == other.type;
    }

    inline bool operator!=(Piece &other) {
        return !operator==(other);
    }

    inline bool empty() {
        return this->type == EMPTY_TYPE;
    }
};

class GameState {
    array<Piece, BOARD_SIZE> board;
    Square pawn_shadow = {-1, -1};
    Square bKingSquare = {-1, -1};
    Square wKingSquare = {-1, -1};

public:
    GameState();
    void operator=(const GameState &other);
    void setup();
    const array<Piece, BOARD_SIZE> &Board() const { return board; }
    void SetBoard(array<Piece, BOARD_SIZE> other_board) {
        board = other_board;
    }
    Square PawnShadow() const { return pawn_shadow; }
    void SetPawnShadow(Square s) { pawn_shadow = s; }
    void set_piece(Square s, Piece p) { board[s.getIdx()] = p; };

    inline bool sameColor(Square from, Square to) {
        return board[from.getIdx()].color == board[to.getIdx()].color;
    }
    inline Square getKingSquare(PieceColor c) const {
        return (c == white) ? wKingSquare : bKingSquare;
    }
    inline void updateKingSquare(Square idx, PieceColor c) {
        Square &king = (c == white) ? wKingSquare : bKingSquare;
        king = idx;
    }
    inline Piece &operator[](Square s) {
        return board[s.getIdx()];
    }
    inline Piece &operator[](short idx) {
        return board[idx];
    }
    
};
