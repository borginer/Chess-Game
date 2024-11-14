#include "chess_game.hpp"

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

move_result ChessGame::Move(Square from, Square to) {
    return doMove(from, to);
}

move_result ChessGame::Move(short from, short to) {
    return doMove(Square(from), Square(to));
}

move_result ChessGame::Move(MovePair pair) {
    return doMove(pair.from, pair.to);
}

move_result ChessGame::MultiMoves(vector<MovePair> moves) {
    move_result res;
    for (MovePair& move: moves) {
        res = doMove(move.from, move.to);
        if (res != move_success) {
            return res;
        }
    }
    return res;
}

move_result ChessGame::doMove(Square from, Square to) {
    Timer t("Move");
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
        commitGameState();
        // printHist();
        switchTurn();
        if (checkMate()) {
            stopGame();
        }
    } else {
        printf("ilegal position reached\n");
        return move_invalid;
    }
    
    if (game_in_progress) {
        return move_success; 
    } else {
        return move_game_over;
    }
}


bool ChessGame::checkMove(Square from, Square to) {
    if (!game_copy.sameColor(game_copy[from].color, turn)) {
        return false;
    }
    return inVec(possibleMoves(from), to);
}


vector<Square> ChessGame::possibleMoves(Square from) {
    switch (game_copy[from].type) {
        case pawn:
            return possiblePawnMoves(from);
        case knight:
            return possibleKnightMoves(from);
        case bishop: 
            return possibleBishopMoves(from);
        case rook:
            return possibleRookMoves(from);
        case queen:
            return possibleQueenMoves(from);
        case king:
            return possibleKingMoves(from);
        case EMPTY_TYPE:
            return {};
        default:
            throw(game_copy[from].type);
    }
}


void ChessGame::moveOnGameStateCopy(Square from, Square to) {
    switch (game_copy[from].type) {
        case pawn:
            handleEnPassant(from, to);
            handlePromotion(from, to);
            break;
        case king:
            handleCastle(from, to);
            game_copy.updateKingSquare(to, game_copy[from].color);
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


bool ChessGame::checkMate() {
    vector<Square> moves;
    for (int idx = 0; idx < squaresAmount; idx++) {
        if (game_copy[idx].color == turn) {
            resetGameStateCopy();
            moves = possibleMoves(idx);

            for (Square& move : moves) {
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


bool ChessGame::legalPosition() {
    bool blackAttackMat[64] = {false};
    bool whiteAttackMat[64] = {false};
    vector<Square> moves;

    for (short idx = 0; idx < squaresAmount; idx++) {
        moves = possibleMoves(Square{idx});
        for (Square& move : moves) {
            if (game_copy[idx].color == white) {
                whiteAttackMat[move.getIdx()] = true;
            }
            if (game_copy[idx].color == black) {
                blackAttackMat[move.getIdx()] = true;
            }
        }
    }

    short whiteKingIdx = game_copy.getKingSquare(white).getIdx();
    short blackKingIdx = game_copy.getKingSquare(black).getIdx();

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


vector<Square> ChessGame::possibleKnightMoves(Square from) {
    vector<Square> ret = {};
    
    for (Square jump: knightMoves) {
        if ((from + jump).onBoard() && 
            !game_copy.sameColor((from + jump), from)) {
            ret.push_back(from + jump);
        }
    }

    return ret;
}


vector<Square> ChessGame::possibleBishopMoves(Square from) {
    Square idx;
    vector<Square> ret = {};
    
    for (Square direct: diagonalMoves) {
        idx = from;
        while ((idx += direct).onBoard()) {
            if (not game_copy.sameColor(from, idx)) {
                ret.push_back(idx);
            }
            if (not game_copy[idx].empty()) {
                break;
            }
        }
    }
    
    return ret;
}


vector<Square> ChessGame::possibleRookMoves(Square from) {
    Square idx;
    vector<Square> ret;
    
    for (Square direct : horizontalMoves) {
        idx = from;
        while ((idx + direct).onBoard()) {
            idx += direct;
            if (!game_copy.sameColor(from, idx)) {
                ret.push_back(idx);
            }
            if (game_copy[idx].type != EMPTY_TYPE) {
                break;
            }
        }
    }
    
    return ret;
}


vector<Square> ChessGame::possibleQueenMoves(Square from) {
    vector<Square> bishop = possibleBishopMoves(from);
    vector<Square> rook = possibleRookMoves(from);
    
    bishop.insert(bishop.end(), rook.begin(), rook.end());
    
    return bishop;
}


vector<Square> ChessGame::possibleKingMoves(Square from) {
    vector<Square> ret;
    Square idx;
    
    for (Square direct: diagonalMoves) {
        idx = from + direct;
        if (idx.onBoard() && !game_copy.sameColor(from, idx)) {
            ret.push_back(idx);
        }
    }

    for (Square direct: horizontalMoves) {
        idx = from + direct;
        if (idx.onBoard() && !game_copy.sameColor(from, idx)) {
            ret.push_back(idx);
        }
    }
    
    queen_castle = false;
    king_castle = false;
    Square kingRookIdx = from + 3 * kingSideMove;
    
    // make more readable!!! after adding tests
    if (game_copy[from].moved == false && 
        game_copy[from + kingSideMove].empty() &&
        game_copy[from + 2 * kingSideMove].empty() &&
        game_copy[kingRookIdx].type == rook &&
        game_copy[kingRookIdx].moved == false) {
            ret.push_back(from + 2 * kingSideMove);
    }
    
    Square queenRookIdx = from + 4 * queenSideMove;
    
    if (game_copy[from].moved == false && 
        game_copy[from + queenSideMove].empty() &&
        game_copy[from + 2 * queenSideMove].empty() &&
        game_copy[from + 3 * queenSideMove].empty() &&
        game_copy[queenRookIdx].type == rook &&
        game_copy[queenRookIdx].moved == false) {
            ret.push_back(from + 2 * queenSideMove);
    }
    
    return ret;
}


vector<Square> ChessGame::possiblePawnMoves(Square from) {
    vector<Square> ret;
    Square* attack = game_copy[from].color == white ? 
         whitePawnAttackMoves : blackPawnAttackMoves;
    Square move = game_copy[from].color == white ?
         whitePawnMoves : blackPawnMoves;
    Square idx;
    Square zero {0, 0};

    while (*attack != zero) {
        idx = from + *attack;

        if (idx.onBoard() && (game_copy[idx].color != 
            EMPTY_PIECE_COLOR || game_copy.PawnShadow() == idx) && 
            !game_copy.sameColor(from, idx)) { 
                ret.push_back(idx);
        }

        attack++;
    }

    Square normal_move = from + move;
    Square double_jump = from + move + move;
    
    bool blocked = !game_copy[normal_move].empty();
    bool jump_blocked = !game_copy[double_jump].empty();    
    bool can_double_jump = !game_copy[from].moved && !blocked && !jump_blocked;

    if (!blocked && normal_move.onBoard()) {
        ret.push_back(normal_move);
    }

    if (can_double_jump) {
        ret.push_back(double_jump);    
    }

    return ret;
}


bool ChessGame::inVec(vector<Square> vec, Square val) {
    for (Square& idx : vec) {
        if (idx == val) { return true; }
    }

    return false;
}


void ChessGame::handleEnPassant(Square from, Square to) {  
    if (game_copy.PawnShadow() == to) {  
        removeEnPassant(to, game_copy[from].color);
    }
    markShadowPawn(from, to);
}


void ChessGame::markShadowPawn(Square from, Square to) {
    game_copy.SetPawnShadow({-1, -1});

    Square move = game_copy[from].color == white ?
         whitePawnMoves : blackPawnMoves; 

    if (move * 2 + from == to) {
        game_copy.SetPawnShadow(from + move);
    }
}


void ChessGame::removeEnPassant(Square to, PieceColor c) {
    Square move = (c == white) ? whitePawnMoves : blackPawnMoves;
    Square idx = to - move;
    game_copy.set_piece(idx, {EMPTY_PIECE_COLOR, EMPTY_TYPE});
}


void ChessGame::handleCastle(Square from, Square to) {
    Square rookIdx;
    king_castle = false;
    queen_castle = false;
    
    if (from + 2 * kingSideMove == to) {
        rookIdx = from + 3 * kingSideMove;
        game_copy[from + kingSideMove] = game_copy[rookIdx];
        game_copy[rookIdx] = { EMPTY_PIECE_COLOR, EMPTY_TYPE };
        king_castle = true;
    }

    if (from + 2 * queenSideMove == to) {
        rookIdx = from + 4 * queenSideMove;
        game_copy[from + queenSideMove] = game_copy[rookIdx];
        game_copy[rookIdx] = { EMPTY_PIECE_COLOR, EMPTY_TYPE };
        queen_castle = true;
    }
}

void ChessGame::handlePromotion(Square from, Square to) {
    if ((game_copy[from].color == white && to.y == 7) ||
        (game_copy[from].color == black && to.y == 0)) {
            game_copy[from].type = queen;
    }
}

void ChessGame::commitGameState() {
    game = game_copy;
}

void ChessGame::resetGameStateCopy() {
    game_copy = game;
}

void ChessGame::stopGame() {
    string winner = (turn == white) ? "black" : "white";
    cout << winner << " wins" << endl; 
    game_in_progress = false;
}

void ChessGame::addMoveToHist(Square from, Square to) {
    this->move_hist.push_back({ from, to, game[from], game[to] });
}

void ChessGame::printHist() {
    for (move_log& move: move_hist) {
        cout << move << endl;
    }
}

void ChessGame::UndoMove() {
    Timer t("Undo");
    if (move_hist.empty()) {
        return;
    }
    move_hist.pop_back();

    vector<move_log> moves = std::move(move_hist);

    reset_game();

    for (const move_log& move: moves) {      
        Move(move.from, move.to);
    }
    move_hist = std::move(moves);
}

bool ChessGame::IsOver() {
    return not game_in_progress;
}

PieceColor ChessGame::WhoseTurn() {
    return turn;
}

ChessGame::ChessGame() {
    reset_game();
}

void ChessGame::reset_game() {
    this->turn = white;
    this->game.setup();
    this->game_in_progress = true;
    this->queen_castle = false;
    this->king_castle = false;
    this->move_hist = {};
}