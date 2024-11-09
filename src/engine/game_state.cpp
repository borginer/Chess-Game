#include "game_state.hpp"

Piece::Piece() {
    this->type = EMPTY_TYPE; 
    this->color = EMPTY_PIECE_COLOR;
    this->moved = false;
}

Piece::Piece(PieceColor c, Type t) {
    this->type = t;
    this->color = c;
    this->moved = false;
}

Piece::Piece(PieceColor c, Type t, bool moved) {
    this->type = t;
    this->color = c;
    this->moved = moved;
}

Piece::Piece(const Piece& other) {
    this->type = other.type;
    this->color = other.color;
    this->moved = other.moved;
}

GameState::GameState() {
    this->board = array<Piece, BOARD_SIZE>();
    pawn_shadow = {-1, -1};
    bKingIdx = -1;
    wKingIdx = -1;
}

void GameState::set_piece(Square s, Piece p) {
    board[s.getIdx()] = p;
}

void GameState::setup() { 
    board = {};
    //rooks
    set_piece({7, 7}, {black, rook});
    set_piece({0, 7}, {black, rook});
    set_piece({0, 0}, {white, rook});
    set_piece({7, 0}, {white, rook});
    //knights
    set_piece({1, 7}, {black, knight});
    set_piece({6, 7}, {black, knight});
    set_piece({1, 0}, {white, knight});
    set_piece({6, 0}, {white, knight});
    //bishops
    set_piece({2, 7}, {black, bishop});
    set_piece({5, 7}, {black, bishop});
    set_piece({2, 0}, {white, bishop});
    set_piece({5, 0}, {white, bishop});
    //queens
    set_piece({3, 7}, {black, queen});
    set_piece({3, 0}, {white, queen});
    //kings
    set_piece({4, 7}, {black, king});
    set_piece({4, 0}, {white, king});
    //pawns
    for(short i = 0; i < 8; ++i) set_piece({i, 6}, {black, pawn});
    for(short i = 0; i < 8; ++i) set_piece({i, 1}, {white, pawn});

    wKingIdx = 4;
    bKingIdx = 60;
}

void GameState::operator=(GameState& other) {
    this->SetBoard(other.Board());
    this->updateKingIdx(other.getKingIdx(white), white);
    this->updateKingIdx(other.getKingIdx(black), black);
    this->SetPawnShadow(other.PawnShadow());
}