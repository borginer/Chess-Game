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

void GameState::setup() { 
    //rooks
    board[0] = {black, rook}; board[7] = {black, rook};
    board[56] = {white, rook}; board[63] = {white, rook};
    //knights
    board[1] = {black, knight}; board[6] = {black, knight};
    board[57] = {white, knight}; board[62] = {white, knight};
    //bishops
    board[2] = {black, bishop}; board[5] = {black, bishop};
    board[58] = {white, bishop}; board[61] = {white, bishop};
    //queens
    board[3] = {black, queen}; board[59] = {white, queen};
    //kings
    board[4] = {black, king}; board[60] = {white, king};
    //pawns
    for(int i = 8; i < 16; ++i) board[i] = {black, pawn};
    for(int i = 48; i < 56; ++i) board[i] = {white, pawn};

    bKingIdx = 4;
    wKingIdx = 60;
}

void GameState::operator=(GameState& other) {
    this->SetBoard(other.Board());
    this->updateKingIdx(other.getKingIdx(white), white);
    this->updateKingIdx(other.getKingIdx(black), black);
    this->SetPawnShadow(other.PawnShadow());
}