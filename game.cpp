#include "game.hpp"

int diagMove[] = {-9, -7, 7, 9}; 
int horizMove[] = {-8, -1, 1, 8};
int horseMove[] = {-17, -15, -10, -6, 6, 10, 15, 17};

// checks indexing of input only
bool checkPoint(square p);


Game::Game(std::vector<Texture2D>& textures){
    this->textures = textures;
    this->turn = white;
    this->board = std::array<types, 64>();
    setup();
}

void Game::SetMarkedSquare(square s){
    marked.x = s.x;
    marked.y = s.y;
}

void Game::setup(){
    turn = white;
    for(types& t: board) t = EMPTY_TYPE;
    //rooks
    board[0] = bRook; board[7] = bRook;
    board[56] = wRook; board[63] = wRook;
    //knights
    board[1] = bKnight; board[6] = bKnight;
    board[57] = wKnight; board[62] = wKnight;
    //bishops
    board[2] = bBishop; board[5] = bBishop;
    board[58] = wBishop; board[61] = wBishop;
    //queens
    board[3] = bQueen; board[59] = wQueen;
    //kings
    board[4] = bKing; board[60] = wKing;
    //pawns
    for(int i = 8; i < 16; ++i) board[i] = bPawn;
    for(int i = 48; i < 56; ++i) board[i] = wPawn;
}

void Game::DrawBoard(){
    int index;        
    DrawTexture(textures[board_txt], sideBarSize, sideBarSize, WHITE);
    if(marked.x != -1) 
        DrawRectangle(sideBarSize + marked.x * figureSize, sideBarSize + marked.y * figureSize, figureSize, figureSize, {255,0,0,130});
    for(int row = 0; row < 8; ++row){
        for(int col = 0; col < 8; ++col){
            index = square{col, row}.getIdx();
            if(board[index] != EMPTY_TYPE){
                DrawTexture(textures[board[index]], sideBarSize + col * figureSize, sideBarSize + row * figureSize, WHITE);
            }
        }
    }
}

void Game::Move(square from, square to){
    if(!checkPoint(from) || !checkPoint(to) || from == to){
        std::cout << "Invalid Square" << std::endl;
        return;
    }
    int idx_from, idx_to;
    idx_from = from.getIdx();
    idx_to = to.getIdx();
    board[idx_to] = board[idx_from];
    board[idx_from] = EMPTY_TYPE;
}

bool checkPoint(square p){
    return !(p.x < 0 || p.x > 7 || p.y < 0 || p.y > 7);
}

std::vector<square> Game::legalMoves(square from){
    std::vector<square> moves = {};
    switch(board[from.getIdx()]){
        case wPawn:   case bPawn:
            break;
        case wKnight: case bKnight:
            break;
        case wBishop: case bBishop:
            break;
        case wRook:   case bRook:
            break;
        case wQueen:  case bQueen:
            break;
        case wKing:   case bKing:
            break;
        case TYPES_LAST: case EMPTY_TYPE:
            break;
    }
    return moves;
}
