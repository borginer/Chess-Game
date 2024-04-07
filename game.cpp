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
#define squaresAmount 64

Game::Game(){
    this->copy_board = std::array<Piece, 64>();
    this->board = std::array<Piece, 64>();
    setup();
}

void Game::printBoard(){
    cout << "types:" << endl;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cout << copy_board[i * 8 + j].type << " ";
        }
        cout << endl;
    }
    cout << "colors:" << endl;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cout << copy_board[i * 8 + j].color << " ";
        }
        cout << endl;
    }
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

    copy_board = board;
    makeMoveOnCopy(idx_from, idx_to);
    printBoard();
    
    if (legalPosition()) {
        board = copy_board;
    }
    else {
        printf("ilegal position reached\n");
    }

    nextTurn();
}

bool Game::checkMove(int from, int to) {
    if (!sameColor(copy_board[from].color, turn)) {
        return false;
    }
    return inVec(possibleMoves(from), to);
}

vector<int> Game::possibleMoves(int from) {
    switch (copy_board[from].type) {
        case pawn:
            return possiblePawnMoves(from);
            break;
        case knight:
            return possibleKnightMoves(from);
            break;
        case bishop: 
            return possibleBishopMoves(from);
            break;
        case rook:
            return possibleRookMoves(from);
            break;
        case queen:
            return possibleQueenMoves(from);
            break;
        case king:
            return possibleKingMoves(from);
            break;
        case EMPTY_TYPE:
            return {};
            break;
        default:
            return {};
    }
}

void Game::makeMoveOnCopy(int from, int to) {
    switch (copy_board[from].type) {
        case pawn:
        handleOnPeasent(from, to);
        break;
        case king:
        handleCastle(from, to);
        break;
        default:
        break;
    }
             
    copy_board[to] = copy_board[from];
    copy_board[from] = {EMPTY_PIECE_COLOR, EMPTY_TYPE};
    copy_board[to].moved = true;
}

void printMat(bool mat[]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cout << mat[i * 8 + j] << " ";
        }
        cout << endl;
    }
}

bool Game::legalPosition() {
    bool blackAttackMat[64] = {false};
    bool whiteAttackMat[64] = {false};
    vector<int> moves;
    Piece p;
    for (int idx = 0; idx < squaresAmount; idx++) {
        moves = possibleMoves(idx);
        for (int move : moves) {
            if (copy_board[idx].color == white) {
                whiteAttackMat[move] = true;
            }
            if (copy_board[idx].color == black) {
                blackAttackMat[move] = true;
            }
        }
    }
    cout << endl << "white:" << endl;
    printMat(whiteAttackMat);
    cout << endl << "black:" << endl;
    printMat(blackAttackMat);
    return true;
}

vector<int> Game::possibleKnightMoves(int from) {
    vector<int> ret = {};
    for (int jump: knightMoves) {
        if (!sameColor(from + jump, from) && onBoard(from + jump)) {
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
        while (onBoard(idx += direct)) {
            //printf("bishop color: %d: idx: %d\n", copy_board[from].color, idx);
            if (!sameColor(from, idx)) {
                ret.push_back(idx);
            }
            if (copy_board[idx].type != EMPTY_TYPE || onEdge(idx)) {
                break;
            }
        }
    }
    return ret;
}

vector<int> Game::possibleRookMoves(int from) {
    int idx;
    vector<int> ret;
    for (int direct : horizontalMoves) {
        idx = from;
        while (onBoard(idx + direct)) {
            idx += direct;
            if (!sameColor(from, idx)) {
                ret.push_back(idx);
            }
            if (copy_board[idx].type != EMPTY_TYPE) {
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
        if (!sameColor(from, idx) && onBoard(idx)) {
            ret.push_back(idx);
        }
    }

    for (int direct: horizontalMoves) {
        idx = from + direct;
        if (!sameColor(from, idx) && onBoard(idx)) {
            ret.push_back(idx);
        }
    }
    queen_castle = false;
    king_castle = false;
    int kingRookIdx = from + 3 * kingSideMove;
    if (copy_board[from].moved == false && 
        copy_board[from + kingSideMove].type == EMPTY_TYPE &&
        copy_board[from + 2 * kingSideMove].type == EMPTY_TYPE &&
        copy_board[kingRookIdx].type == rook &&
        copy_board[kingRookIdx].moved == false) {
            ret.push_back(from + 2 * kingSideMove);
            king_castle = true;
    }
    int queenRookIdx = from + 4 * queenSideMove;
    if (copy_board[from].moved == false && 
        copy_board[from + queenSideMove].type == EMPTY_TYPE &&
        copy_board[from + 2 * queenSideMove].type == EMPTY_TYPE &&
        copy_board[from + 3 * queenSideMove].type == EMPTY_TYPE &&
        copy_board[queenRookIdx].type == rook &&
        copy_board[queenRookIdx].moved == false) {
            ret.push_back(from + 2 * queenSideMove);
            queen_castle = true;
    }
    return ret;
}

vector<int> Game::possiblePawnMoves(int from) {
    vector<int> ret;
    int* attack = copy_board[from].color == white ? 
         whitePawnAttackMoves : blackPawnAttackMoves;
    int* move = copy_board[from].color == white ?
         whitePawnMoves : blackPawnMoves;

    int idx;
    while (*attack) {
        idx = from + *attack;
        if ((copy_board[idx].color != EMPTY_PIECE_COLOR || idx == pawn_shadow) && 
            !sameColor(from, idx) && onBoard(idx)) {
            ret.push_back(idx);
        }
        attack++;
    }

    while (*move) {
        int normal_move = from + *move;
        int double_jump = from + 2 * (*move);

        bool blocked = copy_board[normal_move].type != EMPTY_TYPE;
        bool jump_blocked = copy_board[double_jump].type != EMPTY_TYPE;
        
        bool can_double_jump = !copy_board[from].moved && !blocked && !jump_blocked;

        if (!blocked && onBoard(normal_move)) {
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

void Game::handleOnPeasent(int from, int to) {    
    removePeasent(to, copy_board[from].color);
    markShadowPawn(from, to);
}

void Game::markShadowPawn(int from, int to) {
    pawn_shadow = -1;

    int* move = copy_board[from].color == white ?
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
        copy_board[idx].type = EMPTY_TYPE;
        copy_board[idx].color = EMPTY_PIECE_COLOR;    
    }
}

void Game::handleCastle(int from, int to) {
    int rookIdx;
    if (king_castle) {
        rookIdx = from + 3 * kingSideMove;
        copy_board[from + kingSideMove] = copy_board[rookIdx];
        copy_board[rookIdx].type = EMPTY_TYPE;
        copy_board[rookIdx].color = EMPTY_PIECE_COLOR;
    }

    if (queen_castle) {
        rookIdx = from + 4 * queenSideMove;
        copy_board[from + queenSideMove] = copy_board[rookIdx];
        copy_board[rookIdx].type = EMPTY_TYPE;
        copy_board[rookIdx].color = EMPTY_PIECE_COLOR;
    }
}

bool Game::onEdge(int idx) {
    square s = getSquare(idx);
    return s.x == 0 || s.x == 7 || s.y == 0 || s.y == 7;
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

    copy_board = board;
}