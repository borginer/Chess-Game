#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <iostream>
#include <array>
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

class Game{
private:
    GameState game;
    GameState game_copy;

    vector<move_log> move_hist;
    bool game_in_progress;
    PieceColor turn;
    bool queen_castle = false;
    bool king_castle = false;

    // check if the move is legal
    bool checkMove(Square from, Square to);
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
    // remove the pawn being captured en passant
    void removeEnPassant(Square to, PieceColor c);
    // handle special interactions
    void handleEnPassant(Square from, Square to);
    void handleCastle(Square from, Square to);
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

    public:
    Game();
    // checks legality 
    move_result Move(Square, Square);
    move_result Move(int, int);
    bool IsOver();
    PieceColor WhoseTurn();
    void UndoMove();

    const array<Piece, BOARD_SIZE>& GetBoard() const {return game.Board();};
};

#endif