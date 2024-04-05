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
    if (!from.validBounds() || !to.validBounds() || from == to) {
        std::cout << "Invalid Square" << std::endl;
        return;
    }
    if (!checkMove(from, to)) {
        std::cout << "Invalid Move" << std::endl;
        return;
    }
    int idx_from, idx_to;
    idx_from = from.getIdx();
    idx_to = to.getIdx();
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

bool Game::checkMove(square from, square to) {
    if (!sameColor(board[from.getIdx()].color, turn)) {
        return false;
    }
    switch (board[from.getIdx()].type) {
        case pawn:
            return checkPawnMove(from.getIdx(), to.getIdx());
            break;
        case knight:
            return checkKnightMove(from.getIdx(), to.getIdx());
            break;
        case bishop: 
            return checkBishopMove(from.getIdx(), to.getIdx());
            break;
        case rook:
            return checkRookMove(from.getIdx(), to.getIdx());
            break;
        case queen:
            return checkQueenMove(from.getIdx(), to.getIdx());
            break;
        case king:
            return checkKingMove(from.getIdx(), to.getIdx());
            break;
        case EMPTY_TYPE:
            return false;
            break;
        default:
            return false;
    }
}

bool Game::checkKnightMove(int from, int to) {
    for (int jump: knightMoves) {
        if (from + jump == to && !sameColor(from, to)) {
            return true;
        }
    }
    return false;
}

bool Game::checkBishopMove(int from, int to) {
    int idx;
    for (int direct: diagonalMoves) {
        idx = from;
        while (onBoard(idx)) {
            idx += direct;
            if (idx == to && !sameColor(from, to)) {
                return true;
            }
            if (board[idx].type != EMPTY_TYPE) {
                break;
            }
        }
    }
    return false;
}

bool Game::checkRookMove(int from, int to) {
    int idx;
    for (int direct: horizontalMoves) {
        idx = from;
        while (onBoard(idx)) {
            idx += direct;
            if (idx == to && !sameColor(from, to)) {
                return true;
            }
            if (board[idx].type != EMPTY_TYPE) {
                break;
            }
        }
    }
    return false;
}

bool Game::checkQueenMove(int from, int to) {
    return checkBishopMove(from, to) || checkRookMove(from, to);
}

bool Game::checkKingMove(int from, int to) {
    for (int direct: diagonalMoves) {
        if (from + direct == to && !sameColor(from, to)) {
            return true;
        }
    }

    for (int direct: horizontalMoves) {
        if (from + direct == to && !sameColor(from, to)) {
            return true;
        }
    }

    return false;
}

bool Game::checkPawnMove(int from, int to) {
    int* attack = board[from].color == white ? 
         whitePawnAttackMoves : blackPawnAttackMoves;
    int* move = board[from].color == white ?
         whitePawnMoves : blackPawnMoves;

    while (*attack) {
        if (from + *attack == to && 
        board[to].color != EMPTY_PIECE_COLOR && 
        !sameColor(from, to)) {
            return true;
        }
        attack++;
    }

    while (*move) {
        if ((from + *move == to || 
        ((from + 2 * (*move)) && !board[from].moved)) 
        && !sameColor(from, to)) {
            return true;
        }
        move++;
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
