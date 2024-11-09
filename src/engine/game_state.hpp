#ifndef BOARD_HPP
#define BOARD_HPP

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
    // get index in board array
    inline short getIdx() { return x + 8 * y; }
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
    // checks indexing of input only
    inline bool onBoard() {
        return !(x < 0 || x > 7 || y < 0 || y > 7);
    }
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
    int bKingIdx = -1;
    int wKingIdx = -1;

public:
    GameState();
    void operator=(GameState &other);
    void setup();
    const array<Piece, BOARD_SIZE> &Board() const { return board; }
    void set_piece(Square s, Piece p);
    void SetBoard(array<Piece, BOARD_SIZE> other_board) {
        board = other_board;
    }

    Square PawnShadow() const { return pawn_shadow; }
    void SetPawnShadow(Square s) { pawn_shadow = s; }

    inline int getKingIdx(PieceColor c) {
        return (c == white) ? wKingIdx : bKingIdx;
    }
    inline void updateKingIdx(short idx, PieceColor c) {
        int &king = (c == white) ? wKingIdx : bKingIdx;
        king = idx;
    }
    inline Piece &operator[](Square s) {
        return board[s.getIdx()];
    }
    inline Piece &operator[](short idx) {
        return board[idx];
    }
    inline bool sameColor(short from, short to) {
        return board[from].color == board[to].color;
    }
    inline bool sameColor(Square from, Square to) {
        return sameColor(from.getIdx(), to.getIdx());
    }
    inline bool sameColor(PieceColor p1, PieceColor p2) {
        return p1 == p2;
    }
};

inline Square getSquare(short idx) {
    return Square{idx};
}

#endif