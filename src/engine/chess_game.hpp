#pragma once

#include <string>
#include <chrono>

#include "game_state.hpp"

using namespace std;

enum move_result {
    move_success = 0,
    move_game_over,
    move_out_of_bounds,
    move_invalid,
};

struct move_log {
    Square from;
    Square to;
    Piece moved;
    Piece taken;

    friend ostream& operator<<(ostream& os, const move_log& log) {
        return os << "From: (" << log.from.x << ',' << log.from.y << ')'
        << " To: (" << log.to.x << ',' << log.to.y << ')' << endl
        << "Piece: type - " << log.taken.type << " color - " << log.taken.color;
    };
};

class ChessGame{
private:
    GameState game;
    GameState game_copy;

    vector<move_log> move_hist;
    bool game_in_progress;
    PieceColor turn;
    bool queen_castle;
    bool king_castle;

    // check if the move is legal
    bool checkMove(Square from, Square to);
    // find all possible moves for every piece
    vector<Square> possibleMoves(Square from);
    // helpers for possibleMoves
    vector<Square> possibleKnightMoves(Square from);
    vector<Square> possibleBishopMoves(Square from);
    vector<Square> possibleRookMoves(Square from);
    vector<Square> possibleQueenMoves(Square from);
    vector<Square> possibleKingMoves(Square from);
    vector<Square> possiblePawnMoves(Square from);

    bool inVec(vector<Square> vec, Square val);
    // mark Square if pawn double jumped
    void markShadowPawn(Square from, Square to);
    // remove the pawn being captured en passant
    void removeEnPassant(Square to, PieceColor c);
    // handle special interactions
    void handleEnPassant(Square from, Square to);
    void handleCastle(Square from, Square to);
    void handlePromotion(Square from, Square to);
    // checks that the king is under attack, returns true if it isn't
    bool legalPosition();
    // checks if current turn player lost
    bool checkMate();
    void moveOnGameStateCopy(Square from, Square to);
    void commitGameState();
    void resetGameStateCopy();
    void stopGame();
    void addMoveToHist(Square from, Square to);
    void printHist();
    inline void switchTurn() {
        turn = turn == white ? black : white;
    }
    move_result doMove(Square, Square);
    void reset_game();

    public:
    ChessGame();
    // checks legality 
    move_result Move(Square, Square);
    move_result Move(short, short);
    move_result Move(MovePair);
    // return result of last move executed
    move_result MultiMoves(vector<MovePair>);
    bool IsOver();
    PieceColor WhoseTurn();

    void UndoMove();

    const array<Piece, BOARD_SIZE>& GetBoard() const {return game.Board();};
};

struct Timer {
    string name;
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
    Timer(string name) {
        start = chrono::high_resolution_clock::now();
        this->name = name;
    }
    ~Timer() {
        end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duration = (end - start);
        cout << this->name << " calc time: " << duration.count() << "ms" << endl;
    }
};
