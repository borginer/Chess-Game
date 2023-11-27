#include "game.hpp"

bool checkPoint(std::pair<int, int> p){
    return !(p.first < 0 || p.first > 7 || p.second < 0 || p.second > 7);
}

Game::Game(std::vector<Texture2D>& textures){
    this->textures = textures;
    this->turn = white;
    this->board = std::array<types, 64>();
    setup();
}

void Game::setup(){
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
    for(int row = 0; row < 8; ++row){
        for(int col = 0; col < 8; ++col){
            index = row * 8 + col;
            if(board[index] != EMPTY_TYPE){
                DrawTexture(textures[board[index]], sideBarSize + col * figureSize, sideBarSize + row * figureSize, WHITE);
            }
        }
    }
}

void Game::Move(std::pair<int,int> from, std::pair<int,int> to){
    if(!checkPoint(from) || !checkPoint(to)){
        std::cout << "Invalid Square" << std::endl;
        return;
    }
    int idx_from, idx_to;
    idx_from = from.first + 8 * from.second;
    idx_to = to.first + 8 * to.second;
    board[idx_to] = board[idx_from];
    board[idx_from] = EMPTY_TYPE;
}

