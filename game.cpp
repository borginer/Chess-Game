#include "game.hpp"

static int diagonalMoves[] = {-9, -7, 7, 9}; 
static int horizontalMoves[] = {-8, -1, 1, 8};
static int knightMoves[] = {-17, -15, -10, -6, 6, 10, 15, 17};
static int blackPawnMoves[] = {8, 0};
static int whitePawnMoves[] = {-8, 0};
static int blackPawnAttackMoves[] = {7, 9, 0};
static int whitePawnAttackMoves[] = {-7, -9, 0};


Game::Game(){
    this->board = std::array<Piece, 64>();
    setup();
}

void Game::setup() {
    turn = white;
    for (Piece& p: board) {
        p = {EMPTY_PIECE_COLOR, EMPTY_TYPE};
    }
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

void Game::Move(square from, square to) {
    int idx_from, idx_to;
    idx_from = from.getIdx();
    idx_to = to.getIdx();
    if (!from.validBounds() || !to.validBounds() || from == to) {
        std::cout << "Invalid Square" << std::endl;
        return;
    }
    if (!checkMove(idx_from, idx_to)) {
        std::cout << "Invalid Move" << std::endl;
        return;
    }

    board[idx_to] = board[idx_from];
    board[idx_from] = {EMPTY_PIECE_COLOR, EMPTY_TYPE};
    board[idx_to].moved = true;

    if (turn == white) {
        turn = black;
    }
    else {
        turn = white;
    }
}

bool Game::checkMove(int from, int to) {
    if (!sameColor(board[from].color, turn)) {
        return false;
    }
    switch (board[from].type) {
        case pawn:
            return inVec(possiblePawnMoves(from), to);
            break;
        case knight:
            return inVec(possibleKnightMoves(from), to);
            break;
        case bishop: 
            return inVec(possibleBishopMoves(from), to);
            break;
        case rook:
            return inVec(possibleRookMoves(from), to);
            break;
        case queen:
            return inVec(possibleQueenMoves(from), to);
            break;
        case king:
            return inVec(possibleKingMoves(from), to);
            break;
        case EMPTY_TYPE:
            return false;
            break;
        default:
            return false;
    }
}

vector<int> Game::possibleKnightMoves(int from) {
    vector<int> ret = {};
    for (int jump: knightMoves) {
        if (!sameColor(from + jump, from)) {
            ret.push_back(from + jump);
        }
    }
    return ret;
}

vector<int> Game::possibleBishopMoves(int from) {
    int idx;
    vector<int> ret = {};
    for (int direct: diagonalMoves) {
        idx = from;
        while (onBoard(idx)) {
            idx += direct;
            if (!sameColor(from, idx)) {
                ret.push_back(idx);
            }
            if (board[idx].type != EMPTY_TYPE) {
                break;
            }
        }
    }
    return ret;
}

vector<int> Game::possibleRookMoves(int from) {
    int idx;
    vector<int> ret;
    for (int direct: horizontalMoves) {
        idx = from;
        while (onBoard(idx)) {
            idx += direct;
            if (!sameColor(from, idx)) {
                ret.push_back(idx);
            }
            if (board[idx].type != EMPTY_TYPE) {
                break;
            }
        }
    }
    return ret;
}

vector<int> Game::possibleQueenMoves(int from) {
    vector<int> bishop = possibleBishopMoves(from);
    vector<int> rook = possibleRookMoves(from);
    bishop.insert(bishop.end(), rook.begin(), rook.end());
    return bishop;
}

vector<int> Game::possibleKingMoves(int from) {
    vector<int> ret;
    int idx;
    for (int direct: diagonalMoves) {
        idx = from + direct;
        if (!sameColor(from, idx)) {
            ret.push_back(idx);
        }
    }

    for (int direct: horizontalMoves) {
        idx = from + direct;
        if (!sameColor(from, idx)) {
            ret.push_back(idx);
        }
    }

    return ret;
}

vector<int> Game::possiblePawnMoves(int from) {
    vector<int> ret;
    int* attack = board[from].color == white ? 
         whitePawnAttackMoves : blackPawnAttackMoves;
    int* move = board[from].color == white ?
         whitePawnMoves : blackPawnMoves;

    int idx;
    while (*attack) {
        idx = from + *attack;
        if (board[idx].color != EMPTY_PIECE_COLOR && 
            !sameColor(from, idx)) {
            ret.push_back(idx);
        }
        attack++;
    }

    while (*move) {
        int normal_move = from + *move;
        int double_jump = from + 2 * (*move);

        bool blocked = board[normal_move].type != EMPTY_TYPE;
        bool jump_blocked = board[double_jump].type != EMPTY_TYPE;
        
        bool can_double_jump = !board[from].moved && !blocked && !jump_blocked;

        if (!blocked) {
            ret.push_back(normal_move);
        }

        if (can_double_jump) {
            ret.push_back(double_jump);    
        }
        
        move++;
    }

    return ret;
}

bool Game::inVec(vector<int> vec, int val) {
    for (int idx : vec) {
        if (idx == val) {
            return true;
        }
    }
    return false;
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
