#include "game.hpp"

int diagonalMoves[] = {-9, -7, 7, 9}; 
int horizontalMoves[] = {-8, -1, 1, 8};
int knightMoves[] = {-17, -15, -10, -6, 6, 10, 15, 17};

// checks indexing of input only
bool checkSquare(square s);


Game::Game(){
    this->turn = white;
    this->board = std::array<piece, 64>();
    setup();
}

void Game::setup(){
    turn = white;
    for(piece& t: board) t = {EMPTY_COLOR, EMPTY_TYPE};
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
}

void Game::Move(square from, square to){
    if(!checkSquare(from) || !checkSquare(to) || from == to){
        std::cout << "Invalid Square" << std::endl;
        return;
    }
    if(!checkMove(from, to)){
        std::cout << "Invalid Move" << std::endl;
        return;
    }
    int idx_from, idx_to;
    idx_from = from.getIdx();
    idx_to = to.getIdx();
    board[idx_to] = board[idx_from];
    board[idx_from] = {EMPTY_COLOR, EMPTY_TYPE};
}

bool Game::checkMove(square from, square to){
    switch(board[from.getIdx()].t){
        case pawn:
            return true;
            break;
        case knight:
            return checkKnightMove(from.getIdx(), to.getIdx());
            break;
        case bishop: 
            return true;
            break;
        case rook:
            return true;
            break;
        case queen:
            return true;
            break;
        case king:
            return true;
            break;
        case EMPTY_TYPE:
            return false;
            break;
    }
}

bool Game::checkKnightMove(int from, int to){
    if(board[from].c == board[to].c){return false;}
    for(int x: knightMoves){
        if(from + x == to){return true;}
    }
    return false;
}

bool checkSquare(square p){
    return !(p.x < 0 || p.x > 7 || p.y < 0 || p.y > 7);
}


// std::vector<square> Game::legalMoves(square from){
//     std::vector<square> moves = {};
//         case pawn:
//             break;
//         case knight:
//             break;
//         case bishop: 
//             break;
//         case rook:
//             break;
//         case queen:
//             break;
//         case king:
//             break;
//         case EMPTY_TYPE:
//             break;
//     }
//     return moves;
// }
