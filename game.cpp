#include "game.hpp"

static int diagonalMoves[] = {-9, -7, 7, 9}; 
static int horizontalMoves[] = {-8, -1, 1, 8};
static int knightMoves[] = {-17, -15, -10, -6, 6, 10, 15, 17};
static int blackPawnMoves[] = {8, 0};
static int whitePawnMoves[] = {-8, 0};
static int blackPawnAttackMoves[] = {7, 9, 0};
static int whitePawnAttackMoves[] = {-7, -9, 0};

#define kingSideMove 1
#define queenSideMove -1

#define queenWhiteRook 56
#define kingWhiteRook 63
#define queenBlackRook 0
#define kingBlackRook 7


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
    int idx_from = from.getIdx();
    int idx_to = to.getIdx();

    if (!from.validBounds() || !to.validBounds() || from == to) {
        std::cout << "Square out of bounds" << std::endl;
        return;
    }

    if (!checkMove(idx_from, idx_to)) {
        std::cout << "Invalid Move" << std::endl;
        return;
    }

    makeMove(idx_from, idx_to);
    nextTurn();
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

void Game::makeMove(int from, int to) {
    switch (board[from].type) {
        case pawn:
        printf("pawn\n");
        handleOnPeasent(from, to);
        break;
        case king:
        handleCastle(from, to);
        break;
        default:
        break;
    }
             
    board[to] = board[from];
    board[from] = {EMPTY_PIECE_COLOR, EMPTY_TYPE};
    board[to].moved = true;
}

void Game::handleOnPeasent(int from, int to) {    
    removePeasent(to, board[from].color);
    markShadowPawn(from, to);
}

void Game::handleCastle(int from, int to) {
    int rookIdx;
    if (king_castle) {
        rookIdx = from + 3 * kingSideMove;
        board[from + kingSideMove] = board[rookIdx];
        board[rookIdx].type = EMPTY_TYPE;
        board[rookIdx].color = EMPTY_PIECE_COLOR;
    }

    if (queen_castle) {
        rookIdx = from + 4 * queenSideMove;
        board[from + queenSideMove] = board[rookIdx];
        board[rookIdx].type = EMPTY_TYPE;
        board[rookIdx].color = EMPTY_PIECE_COLOR;
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
    queen_castle = false;
    king_castle = false;
    int kingRookIdx = from + 3 * kingSideMove;
    if (board[from].moved == false && 
        board[from + kingSideMove].type == EMPTY_TYPE &&
        board[from + 2 * kingSideMove].type == EMPTY_TYPE &&
        board[kingRookIdx].type == rook &&
        board[kingRookIdx].moved == false) {
            ret.push_back(from + 2 * kingSideMove);
            king_castle = true;
    }
    int queenRookIdx = from + 4 * queenSideMove;
    if (board[from].moved == false && 
        board[from + queenSideMove].type == EMPTY_TYPE &&
        board[from + 2 * queenSideMove].type == EMPTY_TYPE &&
        board[from + 3 * queenSideMove].type == EMPTY_TYPE &&
        board[queenRookIdx].type == rook &&
        board[queenRookIdx].moved == false) {
            ret.push_back(from + 2 * queenSideMove);
            queen_castle = true;
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
        if ((board[idx].color != EMPTY_PIECE_COLOR || idx == pawn_shadow) && 
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

void Game::markShadowPawn(int from, int to) {
    pawn_shadow = -1;

    int* move = board[from].color == white ?
         whitePawnMoves : blackPawnMoves; 

    if (from + 2*(*move) == to) {
        pawn_shadow = from + *move;
    }
}

void Game::removePeasent(int to, PieceColor c) {
    if (to == pawn_shadow) {
        int* move = (c == white) ?
            whitePawnMoves : blackPawnMoves;
        int idx = to - (*move);
        board[idx].type = EMPTY_TYPE;
        board[idx].color = EMPTY_PIECE_COLOR;    
    }
}


