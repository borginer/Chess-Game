#include "game.hpp"

static chrono::time_point start = std::chrono::time_point<std::chrono::system_clock>::min();
static chrono::time_point reference = std::chrono::time_point<std::chrono::system_clock>::min(); // pain

Square knightMoves[] = {{-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}}; 
Square diagonalMoves[] = {{1, -1}, {1, 1}, {-1, 1}, {-1, -1}};
Square horizontalMoves[] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
Square blackPawnAttackMoves[] = {{-1, 1}, {1, 1}, {0, 0}};
Square whitePawnAttackMoves[] = {{-1, -1}, {1, -1}, {0, 0}};
Square blackPawnMoves = {0, 1};
Square whitePawnMoves = {0, -1};

#define kingSideMove 1
#define queenSideMove -1
#define squaresAmount 64

void Game::Move(Square from, Square to) {
    time();
    resetCopyPosition();
    if (!game_in_progress) {
        cout << "Game Ended" << endl;
        return;
    }
    if (!from.onBoard() || !to.onBoard() || from == to) {
        std::cout << "Square out of bounds" << std::endl;
        time();
        return;
    }
    if (!checkMove(from, to)) {
        std::cout << "Invalid Move" << std::endl;
        time();
        return;
    }

    makeMoveOnCopy(from, to);
    if (legalPosition()) {
        commitPosition();
        nextTurn();
        if (checkMate()) {
            stopGame();
        }
    } else {
        printf("ilegal position reached\n");
    }
    time();
}


bool Game::checkMove(Square from, Square to) {
    if (!sameColor(getPiece(from).color, turn)) {
        return false;
    }
    return inVec(possibleMoves(from), to.getIdx());
}

vector<int> Game::possibleMoves(Square from) {
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


void Game::makeMoveOnCopy(Square from, Square to) {
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
                resetCopyPosition();
                makeMoveOnCopy(idx, move);
                if (legalPosition()) {
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
    return true;
}


vector<int> Game::possibleKnightMoves(Square from) {
    vector<int> ret = {};
    Square s;
    for (Square jump: knightMoves) {
        if (!sameColor((from + jump), from) && (from + jump).onBoard()) {
            ret.push_back((from + jump).getIdx());
        }
    }
    return ret;
}


vector<int> Game::possibleBishopMoves(Square from) {
    Square idx;
    vector<int> ret = {};
    for (Square direct: diagonalMoves) {
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


vector<int> Game::possibleRookMoves(Square from) {
    Square idx;
    vector<int> ret;
    for (Square direct : horizontalMoves) {
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


vector<int> Game::possibleQueenMoves(Square from) {
    vector<int> bishop = possibleBishopMoves(from);
    vector<int> rook = possibleRookMoves(from);
    bishop.insert(bishop.end(), rook.begin(), rook.end());
    return bishop;
}


vector<int> Game::possibleKingMoves(Square from) {
    vector<int> ret;
    Square idx;
    for (Square direct: diagonalMoves) {
        idx = from + direct;
        if (!sameColor(from, idx) && idx.onBoard()) {
            ret.push_back(idx.getIdx());
        }
    }

    for (Square direct: horizontalMoves) {
        idx = from + direct;
        if (!sameColor(from, idx) && idx.onBoard()) {
            ret.push_back(idx.getIdx());
        }
    }
    queen_castle = false;
    king_castle = false;
    Square kingRookIdx = from + 3 * kingSideMove;
    if (getPiece(from).moved == false && 
        getPiece(from + kingSideMove).type == EMPTY_TYPE &&
        getPiece(from + 2 * kingSideMove).type == EMPTY_TYPE &&
        getPiece(kingRookIdx).type == rook &&
        getPiece(kingRookIdx).moved == false) {
            ret.push_back(from.getIdx() + 2 * kingSideMove);
    }
    Square queenRookIdx = from + 4 * queenSideMove;
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


vector<int> Game::possiblePawnMoves(Square from) {
    vector<int> ret;
    Square* attack = getPiece(from).color == white ? 
         whitePawnAttackMoves : blackPawnAttackMoves;
    Square move = getPiece(from).color == white ?
         whitePawnMoves : blackPawnMoves;
    Square idx;

    while ((*attack).getIdx() != 0) {
        idx = from + *attack;
        if ((getPiece(idx).color != EMPTY_PIECE_COLOR || idx == pawn_shadow_copy) && 
            !sameColor(from, idx) && idx.onBoard()) { 
                ret.push_back(idx.getIdx());
        }
        attack++;
    }

    Square normal_move = from + move;
    Square double_jump = from + move + move;
    
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


void Game::handleOnPeasent(Square from, Square to) {    
    removePeasent(to, getPiece(from).color);
    markShadowPawn(from, to);
}


void Game::markShadowPawn(Square from, Square to) {
    pawn_shadow_copy = -1;

    Square move = getPiece(from).color == white ?
         whitePawnMoves : blackPawnMoves; 

    if (from + 2*(move.getIdx()) == to) {
        pawn_shadow_copy = from + move.getIdx();
    }
}


void Game::removePeasent(Square to, PieceColor c) {
    if (to == pawn_shadow_copy) {
        Square move = (c == white) ?
            whitePawnMoves : blackPawnMoves;
        
        Square idx = to - move;
        getPiece(idx).type = EMPTY_TYPE;
        getPiece(idx).color = EMPTY_PIECE_COLOR;    
    }
}


void Game::handleCastle(Square from, Square to) {
    Square rookIdx;
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

void Game::stopGame() {
    string winner = (turn == white) ? "black" : "white";
    cout << winner << " wins" << endl; 
    game_in_progress = false;
}

Game::Game(){
    this->copy_board = array<Piece, 64>();
    this->board = array<Piece, 64>();
    setup();
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


void Game::time() {
    if (start != reference) {
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duration = end - start;
        cout << "Move calc time: " << duration.count() << "ms" << endl;
        start = std::chrono::time_point<std::chrono::system_clock>::min();
    } else {
        start = chrono::high_resolution_clock::now();
    }
}