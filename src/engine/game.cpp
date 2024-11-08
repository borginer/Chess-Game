#include "game.hpp"

static Square knightMoves[] = {{-2, -1}, {-2, 1}, {2, -1}, {2, 1},
                               {-1, -2}, {-1, 2}, {1, -2}, {1, 2}}; 
static Square diagonalMoves[] = {{1, -1}, {1, 1}, {-1, 1}, {-1, -1}};
static Square horizontalMoves[] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
static Square blackPawnAttackMoves[] = {{-1, -1}, {1, -1}, {0, 0}};
static Square whitePawnAttackMoves[] = {{-1, 1}, {1, 1}, {0, 0}};
static Square blackPawnMoves = {0, -1};
static Square whitePawnMoves = {0, 1};

#define kingSideMove 1
#define queenSideMove -1
#define squaresAmount 64

move_result Game::Move(Square from, Square to) {
    return doMove(from, to);
}

move_result Game::Move(int from, int to) {
    return doMove(Square(from), Square(to));
}

move_result Game::doMove(Square from, Square to) {
    //time calcs
    auto start = chrono::high_resolution_clock::now();

    resetGameStateCopy();
    if (!game_in_progress) {
        cout << "Game Ended" << endl;
        return move_game_over;
    }
    if (!from.onBoard() || !to.onBoard()) {
        std::cout << "Square out of bounds" << std::endl;
        return move_out_of_bounds;
    }
    if (from == to || !checkMove(from, to)) {
        std::cout << "Invalid Move" << std::endl;
        return move_invalid;
    }

    moveOnGameStateCopy(from, to);
    if (legalPosition()) {
        addMoveToHist(from, to);
        printHist();
        commitGameState();
        switchTurn();
        if (checkMate()) {
            stopGame();
        }
    } else {
        printf("ilegal position reached\n");
        return move_invalid;
    }
    
    //time calcs
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = (end - start);
    cout << "Move calc time: " << duration.count() << "ms" << endl;
    if (game_in_progress) {
        return move_success; 
    } else {
        return move_game_over;
    }
}


bool Game::checkMove(Square from, Square to) {
    if (!game_copy.sameColor(game_copy[from].color, turn)) {
        return false;
    }
    return inVec(possibleMoves(from), to.getIdx());
}


vector<int> Game::possibleMoves(Square from) {
    switch (game_copy[from].type) {
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


void Game::moveOnGameStateCopy(Square from, Square to) {
    switch (game_copy[from].type) {
        case pawn:
        handleEnPassant(from.getIdx(), to.getIdx());
        break;
        case king:
        handleCastle(from.getIdx(), to.getIdx());
        game_copy.updateKingIdx(to.getIdx(), game_copy[from].color);
        break;
        default:
        break;
    }
             
    game_copy[to] = game_copy[from];
    game_copy[from] = {EMPTY_PIECE_COLOR, EMPTY_TYPE};
    game_copy[to].moved = true;
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
    resetGameStateCopy();
    for (int idx = 0; idx < squaresAmount; idx++) {
        if (game_copy[idx].color == turn) {
            resetGameStateCopy();
            moves = possibleMoves(idx);

            for (int move : moves) {
                resetGameStateCopy();
                moveOnGameStateCopy(idx, move);
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

    for (int idx = 0; idx < squaresAmount; idx++) {
        moves = possibleMoves(getSquare(idx));
        for (int move : moves) {
            if (game_copy[idx].color == white) {
                whiteAttackMat[move] = true;
            }
            if (game_copy[idx].color == black) {
                blackAttackMat[move] = true;
            }
        }
    }

    int whiteKingIdx = game_copy.getKingIdx(white);
    int blackKingIdx = game_copy.getKingIdx(black);

    if ((turn == black && whiteAttackMat[blackKingIdx]) || 
        (turn == white && blackAttackMat[whiteKingIdx])) {
        king_castle = false;
        queen_castle = false;
        return false;
    }

    if ((turn == black && king_castle &&
         ((whiteAttackMat[blackKingIdx + kingSideMove]) ||
         whiteAttackMat[blackKingIdx + 2 * kingSideMove])) ||
         
         (turn == white && king_castle &&
         ((whiteAttackMat[whiteKingIdx + kingSideMove] ||
         whiteAttackMat[whiteKingIdx + 2 * kingSideMove])))
         ) {
        king_castle = false;
        return false;
    }

    if ((turn == black && queen_castle &&
         ((whiteAttackMat[blackKingIdx + queenSideMove]) ||
         whiteAttackMat[blackKingIdx + 2 * queenSideMove])) ||
         (turn == white && queen_castle &&
         ((whiteAttackMat[whiteKingIdx + queenSideMove] ||
         whiteAttackMat[whiteKingIdx + 2 * queenSideMove])))
         ) {
        queen_castle = false;
        return false;
    }

    return true;
}


vector<int> Game::possibleKnightMoves(Square from) {
    vector<int> ret = {};
    
    for (Square jump: knightMoves) {
        if ((from + jump).onBoard() && 
            !game_copy.sameColor((from + jump), from)) {
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
            if (!game_copy.sameColor(from, idx)) {
                ret.push_back(idx.getIdx());
            }
            if (game_copy[idx.getIdx()].type != EMPTY_TYPE) {
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
            if (!game_copy.sameColor(from, idx)) {
                ret.push_back(idx.getIdx());
            }
            if (game_copy[idx].type != EMPTY_TYPE) {
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
        if (idx.onBoard() && !game_copy.sameColor(from, idx)) {
            ret.push_back(idx.getIdx());
        }
    }

    for (Square direct: horizontalMoves) {
        idx = from + direct;
        if (idx.onBoard() && !game_copy.sameColor(from, idx)) {
            ret.push_back(idx.getIdx());
        }
    }
    
    queen_castle = false;
    king_castle = false;
    Square kingRookIdx = from + 3 * kingSideMove;
    
    if (game_copy[from].moved == false && 
        game_copy[from + kingSideMove].type == EMPTY_TYPE &&
        game_copy[from + 2 * kingSideMove].type == EMPTY_TYPE &&
        game_copy[kingRookIdx].type == rook &&
        game_copy[kingRookIdx].moved == false) {
            ret.push_back(from.getIdx() + 2 * kingSideMove);
    }
    
    Square queenRookIdx = from + 4 * queenSideMove;
    
    if (game_copy[from].moved == false && 
        game_copy[from + queenSideMove].type == EMPTY_TYPE &&
        game_copy[from + 2 * queenSideMove].type == EMPTY_TYPE &&
        game_copy[from + 3 * queenSideMove].type == EMPTY_TYPE &&
        game_copy[queenRookIdx].type == rook &&
        game_copy[queenRookIdx].moved == false) {
            ret.push_back(from.getIdx() + 2 * queenSideMove);
    }
    
    return ret;
}


vector<int> Game::possiblePawnMoves(Square from) {
    vector<int> ret;
    Square* attack = game_copy[from].color == white ? 
         whitePawnAttackMoves : blackPawnAttackMoves;
    Square move = game_copy[from].color == white ?
         whitePawnMoves : blackPawnMoves;
    Square idx;

    while ((*attack).getIdx() != 0) {
        idx = from + *attack;
        if (idx.onBoard() && (game_copy[idx].color != 
            EMPTY_PIECE_COLOR || game_copy.PawnShadow() == idx) && 
            !game_copy.sameColor(from, idx)) { 
                ret.push_back(idx.getIdx());
        }
        attack++;
    }

    Square normal_move = from + move;
    Square double_jump = from + move + move;
    
    bool blocked = game_copy[normal_move].type != EMPTY_TYPE;
    bool jump_blocked = game_copy[double_jump].type != EMPTY_TYPE;    
    bool can_double_jump = !game_copy[from].moved && !blocked && !jump_blocked;

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


void Game::handleEnPassant(Square from, Square to) {    
    removeEnPassant(to, game_copy[from].color);
    markShadowPawn(from, to);
}


void Game::markShadowPawn(Square from, Square to) {
    game_copy.SetPawnShadow({-1, -1});

    Square move = game_copy[from].color == white ?
         whitePawnMoves : blackPawnMoves; 

    if (from + 2*(move.getIdx()) == to) {
        game_copy.SetPawnShadow(from + move);
    }
}


void Game::removeEnPassant(Square to, PieceColor c) {
    if (game_copy.PawnShadow() == to) {
        Square move = (c == white) ?
            whitePawnMoves : blackPawnMoves;
        Square idx = to - move;
        
        game_copy[idx].type = EMPTY_TYPE;
        game_copy[idx].color = EMPTY_PIECE_COLOR;    
    }
}


void Game::handleCastle(Square from, Square to) {
    Square rookIdx;
    king_castle = false;
    queen_castle = false;
    
    if (from + 2 * kingSideMove == to) {
        rookIdx = from + 3 * kingSideMove;
        game_copy[from + kingSideMove] = game_copy[rookIdx];
        game_copy[rookIdx].type = EMPTY_TYPE;
        game_copy[rookIdx].color = EMPTY_PIECE_COLOR;
        king_castle = true;
    }

    if (from + 2 *queenSideMove == to) {
        rookIdx = from + 4 * queenSideMove;
        game_copy[from + queenSideMove] = game_copy[rookIdx];
        game_copy[rookIdx].type = EMPTY_TYPE;
        game_copy[rookIdx].color = EMPTY_PIECE_COLOR;
        queen_castle = true;
    }
}


Game::Game() {
    this->turn = white;
    this->game.setup();
    this->game_in_progress = true;
}

void Game::commitGameState() {
    game = game_copy;
}

void Game::resetGameStateCopy() {
    game_copy = game;
}

void Game::stopGame() {
    string winner = (turn == white) ? "black" : "white";
    cout << winner << " wins" << endl; 
    game_in_progress = false;
}

void Game::addMoveToHist(Square from, Square to) {
    move_log newMove = { from, to, game[from], game[to] };
    this->move_hist.push_back(newMove);
}

void Game::printHist() {
    for (move_log& move: move_hist) {
        cout << move << endl;
    }
}

void Game::UndoMove() {
    move_log lastMove;
    if (!move_hist.empty()) {
        lastMove = move_hist.back();
    } else {
        return;
    }
    switchTurn();

    game[lastMove.from] = lastMove.moved;
    game[lastMove.to] = lastMove.taken;
    move_hist.pop_back();
}

bool Game::IsOver() {
    return not game_in_progress;
}

PieceColor Game::WhoseTurn() {
    return turn;
}