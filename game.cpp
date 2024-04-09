#include "game.hpp"

//static int diagonalMoves2[] = {-9, -7, 7, 9}; 
//static int horizontalMoves[] = {-8, -1, 1, 8};
//static int knightMoves[] = {-17, -15, -10, -6, 6, 10, 15, 17};
//static int blackPawnMoves[] = {8, 0};
//static int whitePawnMoves[] = {-8, 0};
//static int blackPawnAttackMoves[] = {7, 9, 0};
//static int whitePawnAttackMoves[] = {-7, -9, 0};

square knightMoves[] = {{-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}}; 
square diagonalMoves[] = {{1, -1}, {1, 1}, {-1, 1}, {-1, -1}};
square horizontalMoves[] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
square blackPawnAttackMoves[] = {{-1, 1}, {1, 1}, {0, 0}};
square whitePawnAttackMoves[] = {{-1, -1}, {1, -1}, {0, 0}};
square blackPawnMoves = {0, 1};
square whitePawnMoves = {0, -1};

#define kingSideMove 1
#define queenSideMove -1
#define squaresAmount 64

Game::Game(){
    this->copy_board = array<Piece, 64>();
    this->board = array<Piece, 64>();
    setup();
}

void Game::Move(square from, square to) {
    auto start = chrono::high_resolution_clock::now();
    int idx_from = from.getIdx();
    int idx_to = to.getIdx();
    copy_board = board;
    
    if (!game_in_progress) {
        cout << "Game Ended" << endl;
        return;
    }

    if (!from.onBoard() || !to.onBoard() || from == to) {
        std::cout << "Square out of bounds" << std::endl;
        return;
    }

    resetCopyPosition();

    if (!checkMove(from, to)) {
        std::cout << "Invalid Move" << std::endl;
        return;
    }

    makeMoveOnCopy(idx_from, idx_to);
    
    if (legalPosition()) {
        commitPosition();
        nextTurn();
        if (checkMate()) {
            string clr = (turn == white) ? "white" : "black";
            cout << clr << " wins" << endl; 
            game_in_progress = false;
        }
    }
    else {
        printf("ilegal position reached\n");
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;
    cout << "Move calc time: " << duration.count() << "ms" << endl;
}

bool Game::checkMove(square from, square to) {
    if (!sameColor(getPiece(from).color, turn)) {
        return false;
    }
    return inVec(possibleMoves(from), to.getIdx());
}

vector<int> Game::possibleMoves(square from) {
    switch (getPiece(from).type) {
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

void Game::makeMoveOnCopy(square from, square to) {
    switch (getPiece(from).type) {
        case pawn:
        handleOnPeasent(from.getIdx(), to.getIdx());
        break;
        case king:
        handleCastle(from.getIdx(), to.getIdx());
        updateKingIdxCopy(to.getIdx(), getPiece(from).color);
        break;
        default:
        break;
    }
             
    getPiece(to) = getPiece(from);
    getPiece(from) = {EMPTY_PIECE_COLOR, EMPTY_TYPE};
    getPiece(to).moved = true;
}

void printMat(bool mat[]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cout << mat[i * 8 + j] << " ";
        }
        cout << endl;
    }
}

bool Game::checkMate() {
    vector<int> moves;
    copy_board = board;
    for (int idx = 0; idx < squaresAmount; idx++) {
        if (getPiece(idx).color == turn) {
            resetCopyPosition();
            moves = possibleMoves(idx);
            for (int move : moves) {
                copy_board = board;
                makeMoveOnCopy(idx, move);
                if (legalPosition()) {
                    // cout << idx << " move: " << move << endl;
                    return false;
                }
            }
        }
    }
    return true;
}

bool Game::legalPosition() {
    bool blackAttackMat[64] = {false};
    bool whiteAttackMat[64] = {false};
    vector<int> moves;
    Piece p;
    for (int idx = 0; idx < squaresAmount; idx++) {
        moves = possibleMoves(getSquare(idx));
        for (int move : moves) {
            if (getPiece(idx).color == white) {
                whiteAttackMat[move] = true;
            }
            if (getPiece(idx).color == black) {
                blackAttackMat[move] = true;
            }
        }
    }
    if ((turn == black && whiteAttackMat[bKingIdxCopy]) || 
        (turn == white && blackAttackMat[wKingIdxCopy])) {
        king_castle = false;
        queen_castle = false;
        return false;
    }

    if ((turn == black && king_castle &&
         ((whiteAttackMat[bKingIdxCopy + kingSideMove]) ||
         whiteAttackMat[bKingIdxCopy + 2 * kingSideMove])) ||
         (turn == white && king_castle &&
         ((whiteAttackMat[wKingIdxCopy + kingSideMove] ||
         whiteAttackMat[wKingIdxCopy + 2 * kingSideMove])))
         ) {
        king_castle = false;
        return false;
    }

    if ((turn == black && queen_castle &&
         ((whiteAttackMat[bKingIdxCopy + queenSideMove]) ||
         whiteAttackMat[bKingIdxCopy + 2 * queenSideMove])) ||
         (turn == white && queen_castle &&
         ((whiteAttackMat[wKingIdxCopy + queenSideMove] ||
         whiteAttackMat[wKingIdxCopy + 2 * queenSideMove])))
         ) {
        queen_castle = false;
        return false;
    }
    // cout << endl << "white:" << endl;
    // printMat(whiteAttackMat);
    // cout << endl << "black:" << endl;
    // printMat(blackAttackMat);
    // cout << "black king idx: " << bKingIdxCopy << endl;
    return true;
}

vector<int> Game::possibleKnightMoves(square from) {
    vector<int> ret = {};
    square s;
    for (square jump: knightMoves) {
        if (!sameColor((from + jump), from) && (from + jump).onBoard()) {
            ret.push_back((from + jump).getIdx());
        }
    }
    // cout << "x: " << from.x << "y: " << from.y << endl;
    // for (int x : ret) {
    //     cout << x << " ";
    // }
    return ret;
}

vector<int> Game::possibleBishopMoves(square from) {
    square idx;
    vector<int> ret = {};
    for (square direct: diagonalMoves) {
        idx = from;
        while ((idx += direct).onBoard()) {
            if (!sameColor(from, idx)) {
                ret.push_back(idx.getIdx());
            }
            if (getPiece(idx.getIdx()).type != EMPTY_TYPE) {
                break;
            }
        }
    }
    return ret;
}

vector<int> Game::possibleRookMoves(square from) {
    square idx;
    vector<int> ret;
    for (square direct : horizontalMoves) {
        idx = from;
        while ((idx + direct).onBoard()) {
            idx += direct;
            if (!sameColor(from, idx)) {
                ret.push_back(idx.getIdx());
            }
            if (getPiece(idx).type != EMPTY_TYPE) {
                break;
            }
        }
    }
    return ret;
}

vector<int> Game::possibleQueenMoves(square from) {
    vector<int> bishop = possibleBishopMoves(from);
    vector<int> rook = possibleRookMoves(from);
    bishop.insert(bishop.end(), rook.begin(), rook.end());
    return bishop;
}

vector<int> Game::possibleKingMoves(square from) {
    vector<int> ret;
    square idx;
    for (square direct: diagonalMoves) {
        idx = from + direct;
        if (!sameColor(from, idx) && idx.onBoard()) {
            ret.push_back(idx.getIdx());
        }
    }

    for (square direct: horizontalMoves) {
        idx = from + direct;
        if (!sameColor(from, idx) && idx.onBoard()) {
            ret.push_back(idx.getIdx());
        }
    }
    queen_castle = false;
    king_castle = false;
    square kingRookIdx = from + 3 * kingSideMove;
    if (getPiece(from).moved == false && 
        getPiece(from + kingSideMove).type == EMPTY_TYPE &&
        getPiece(from + 2 * kingSideMove).type == EMPTY_TYPE &&
        getPiece(kingRookIdx).type == rook &&
        getPiece(kingRookIdx).moved == false) {
            ret.push_back(from.getIdx() + 2 * kingSideMove);
    }
    square queenRookIdx = from + 4 * queenSideMove;
    if (getPiece(from).moved == false && 
        getPiece(from + queenSideMove).type == EMPTY_TYPE &&
        getPiece(from + 2 * queenSideMove).type == EMPTY_TYPE &&
        getPiece(from + 3 * queenSideMove).type == EMPTY_TYPE &&
        getPiece(queenRookIdx).type == rook &&
        getPiece(queenRookIdx).moved == false) {
            ret.push_back(from.getIdx() + 2 * queenSideMove);
    }
    return ret;
}

vector<int> Game::possiblePawnMoves(square from) {
    vector<int> ret;
    square* attack = getPiece(from).color == white ? 
         whitePawnAttackMoves : blackPawnAttackMoves;
    square move = getPiece(from).color == white ?
         whitePawnMoves : blackPawnMoves;

    square idx;
    while ((*attack).getIdx() != 0) {
        idx = from + *attack;
        cout << "from color: " << getPiece(from).color << " idx color: " << getPiece(idx).color << endl;
        cout << "shadow: " << pawn_shadow_copy << endl;
        if ((getPiece(idx).color != EMPTY_PIECE_COLOR || idx.getIdx() == pawn_shadow_copy) && 
            !sameColor(from, idx) && idx.onBoard()) { 
                ret.push_back(idx.getIdx());
        }
        attack++;
    }

    square normal_move = from + move;
    square double_jump = from + move + move;

    bool blocked = getPiece(normal_move).type != EMPTY_TYPE;
    bool jump_blocked = getPiece(double_jump).type != EMPTY_TYPE;
        
    bool can_double_jump = !getPiece(from).moved && !blocked && !jump_blocked;

    if (!blocked && normal_move.onBoard()) {
        ret.push_back(normal_move.getIdx());
    }

    if (can_double_jump) {
        ret.push_back(double_jump.getIdx());    
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
    removePeasent(to, getPiece(from).color);
    markShadowPawn(from, to);
}

void Game::markShadowPawn(int from, int to) {
    pawn_shadow_copy = -1;

    square move = getPiece(from).color == white ?
         whitePawnMoves : blackPawnMoves; 

    if (from + 2*(move.getIdx()) == to) {
        pawn_shadow_copy = from + move.getIdx();
        cout << "mark shadow pawn: " << pawn_shadow_copy << " color: " << getPiece(from).color << endl;
    }
}

void Game::removePeasent(square to, PieceColor c) {
    cout << "shadow: " << pawn_shadow_copy << " to idx: " << to.getIdx() << endl;
    if (to.getIdx() == pawn_shadow_copy) {
        square move = (c == white) ?
            whitePawnMoves : blackPawnMoves;
        square idx = to - move;
        getPiece(idx).type = EMPTY_TYPE;
        getPiece(idx).color = EMPTY_PIECE_COLOR;    
    }
}

void Game::handleCastle(square from, square to) {
    square rookIdx;
    king_castle = false;
    queen_castle = false;
    if (from + 2 * kingSideMove == to) {
        rookIdx = from + 3 * kingSideMove;
        getPiece(from + kingSideMove) = getPiece(rookIdx);
        getPiece(rookIdx).type = EMPTY_TYPE;
        getPiece(rookIdx).color = EMPTY_PIECE_COLOR;
        king_castle = true;
    }

    if (from + 2 *queenSideMove == to) {
        rookIdx = from + 4 * queenSideMove;
        getPiece(from + queenSideMove) = getPiece(rookIdx);
        getPiece(rookIdx).type = EMPTY_TYPE;
        getPiece(rookIdx).color = EMPTY_PIECE_COLOR;
        queen_castle = true;
    }
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

    bKingIdx = 4;
    wKingIdx = 60;
    game_in_progress = true;

}

void Game::calcKingIdx() {
    for (int i = 0; i < squaresAmount; i++) {
        if (board[i].type == king) {
            if (board[i].color == black) {
                bKingIdx = i;
            } else {
                wKingIdx = i;
            }

        }
    }
}

void Game::commitKingIdx() {
    wKingIdx = wKingIdxCopy;
    bKingIdx = bKingIdxCopy;
}

void Game::resetKingIdxCopy() {
    wKingIdxCopy = wKingIdx;
    bKingIdxCopy = bKingIdx;
}

void Game::commitPosition() {
    board = copy_board;
    commitKingIdx();
    pawn_shadow = pawn_shadow_copy;
}

void Game::resetCopyPosition() {
    copy_board = board;
    resetKingIdxCopy();
    pawn_shadow_copy = pawn_shadow;
}

void Game::printBoard(){
    cout << "types:" << endl;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cout << getPiece(i * 8 + j).type << " ";
        }
        cout << endl;
    }
    cout << "colors:" << endl;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cout << getPiece(i * 8 + j).color << " ";
        }
        cout << endl;
    }
}