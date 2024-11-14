#include <gtest/gtest.h>
#include <iostream>

#include "engine/chess_game.hpp"

TEST(BasicIntegration, BasicMoveTest) {
    ChessGame game{};
    EXPECT_EQ(game.Move({4, 1}, {4, 3}), move_success);
}

TEST(BasicIntegration, BasicCheckMate) {
    ChessGame game{};
    game.Move({4, 1}, {4, 3});
    game.Move({4, 6}, {4, 4});
    game.Move({5, 0}, {2, 3});
    game.Move({5, 7}, {2, 4});
    game.Move({3, 0}, {7, 4});
    game.Move({6, 7}, {5, 5});
    EXPECT_EQ(game.Move({7, 4}, {5, 6}), move_game_over);
    EXPECT_EQ(game.IsOver(), true);
}

TEST(BasicIntegration, BasicMovesOutOfBounds) {
    ChessGame game{};
    EXPECT_EQ(game.Move({9, 0}, {0, 0}), move_out_of_bounds);
    EXPECT_EQ(game.Move({0, 0}, {0, 9}), move_out_of_bounds);
    EXPECT_EQ(game.Move({-1, 0}, {0, 0}), move_out_of_bounds);
    EXPECT_EQ(game.Move({-1, -1}, {-1, -1}), move_out_of_bounds);
    EXPECT_EQ(game.Move({-3, -3}, {-5, -5}), move_out_of_bounds);
    EXPECT_EQ(game.Move({8, 8}, {0, 0}), move_out_of_bounds);
    EXPECT_EQ(game.Move({0, 0}, {8, 0}), move_out_of_bounds);
    EXPECT_EQ(game.Move({9, 0}, {-3, -3}), move_out_of_bounds);
}

TEST(BasicIntegration, InvalidKnightMoves) {
    ChessGame game{};
    bool all_invalid = true;
    Square leftWhiteKnight = {1, 0};
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (game.Move(leftWhiteKnight, {i, j}) != move_invalid) {
                game.UndoMove();
                if (j != 2 || (i != 0 && i != 2)) {
                    all_invalid = false;
                }
            }
        }
    }

    EXPECT_EQ(all_invalid, true);
}

TEST(BasicIntegration, InvalidRookMoves) {
    ChessGame game{};
    bool all_invalid = true;
    Square rightBlackRook = {7, 7};

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (game.Move(rightBlackRook, {i, j}) != move_invalid) {
                all_invalid = false;
            }
        }
    }

    EXPECT_EQ(all_invalid, true);
}

TEST(BasicIntegration, InvalidQueenMoves) {
    ChessGame game{};
    bool all_invalid = true;
    Square blackQueen = {3, 7};

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (game.Move(blackQueen, {i, j}) != move_invalid) {
                all_invalid = false;
            }
        }
    }

    EXPECT_EQ(all_invalid, true);
}

TEST(BasicIntegration, InvalidKingMoves) {
    ChessGame game{};
    bool all_invalid = true;
    Square whiteKing = {4, 0};

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (game.Move(whiteKing, {i, j}) != move_invalid) {
                all_invalid = false;
            }
        }
    }

    EXPECT_EQ(all_invalid, true);
}

TEST(BasicIntegration, BasicMultiMovesSuccess) {
    ChessGame game{};
    vector<MovePair> moves = {
        {{4, 1}, {4, 3}}, {{4, 6}, {4, 4}}, {{5, 0}, {2, 3}}, 
        {{5, 7}, {2, 4}}, {{3, 0}, {7, 4}}, {{6, 7}, {5, 5}}, 
        {{7, 4}, {5, 6}}
    };
    EXPECT_EQ(game.MultiMoves(moves), move_game_over);
    EXPECT_EQ(game.IsOver(), true);
}

TEST(BasicIntegration, BasicMultiMovesFail) {
    ChessGame game{};
    vector<MovePair> moves = {
        {{4, 1}, {4, 3}}, {{4, 6}, {4, 4}}, {{5, 0}, {2, 3}}, 
        {{5, 7}, {2, 4}}, {{3, 0}, {7, 5}}, {{6, 7}, {5, 5}}, 
        {{7, 4}, {5, 6}}
    };
    EXPECT_EQ(game.MultiMoves(moves), move_invalid);
    EXPECT_EQ(game.IsOver(), false);
}

TEST(BasicIntegration, BasicOnPassant) {
    ChessGame game{};
    vector<MovePair> moves = {
        {{4, 1}, {4, 3}}, {{4, 6}, {4, 5}}, {{4, 3}, {4, 4}}, 
        {{3, 6}, {3, 4}}, {{4, 4}, {3, 5}}
    };
    EXPECT_EQ(game.MultiMoves(moves), move_success);
}

TEST(BasicIntegration, OnPassantUndoReplay) {
    ChessGame game{};
    vector<MovePair> moves = {
        {{4, 1}, {4, 3}}, {{4, 6}, {4, 5}}, {{4, 3}, {4, 4}}, 
        {{3, 6}, {3, 4}}, {{4, 4}, {3, 5}}
    };
    EXPECT_EQ(game.MultiMoves(moves), move_success);
    game.UndoMove();
    EXPECT_EQ(game.Move({4, 4}, {3, 5}), move_success);
}


TEST(BasicIntegration, BasicOnPassantFail) {
    ChessGame game{};
    vector<MovePair> moves = {
        {{4, 1}, {4, 3}}, {{4, 6}, {4, 5}}, {{4, 3}, {4, 4}}, 
        {{3, 6}, {3, 4}}, {{0, 1}, {0, 2}}, {{0, 6}, {0, 5}}, {{4, 4}, {3, 5}}
    };
    EXPECT_EQ(game.MultiMoves(moves), move_invalid);
}

TEST(BasicIntegration, BasicCastleKingSide) {
    ChessGame game{};
    vector<MovePair> moves = {
        {{4, 1}, {4, 3}}, {{4, 6}, {4, 4}}, {{5, 0}, {2, 3}}, 
        {{5, 7}, {2, 4}}, {{6, 0}, {5, 2}}, {{6, 7}, {5, 5}},
        {{4, 0}, {6, 0}}, {{4, 7}, {6, 7}}
    };
    EXPECT_EQ(game.MultiMoves(moves), move_success);
}

TEST(BasicIntegration, BasicCastleKingSideUndo) {
    ChessGame game{};
    vector<MovePair> moves = {
        {{4, 1}, {4, 3}}, {{4, 6}, {4, 4}}, {{5, 0}, {2, 3}}, 
        {{5, 7}, {2, 4}}, {{6, 0}, {5, 2}}, {{6, 7}, {5, 5}},
        {{4, 0}, {6, 0}}, {{4, 7}, {6, 7}}
    };
    EXPECT_EQ(game.MultiMoves(moves), move_success);
    game.UndoMove();
    game.UndoMove();
    EXPECT_EQ(game.Move({{4, 0}, {6, 0}}), move_success);
    EXPECT_EQ(game.Move({{4, 7}, {6, 7}}), move_success);
}

TEST(BasicIntegration, BasicCastleQueenSide) {
    ChessGame game{};
    vector<MovePair> moves = {
        {{3, 1}, {3, 3}}, {{3, 6}, {3, 4}}, {{3, 0}, {3, 2}}, 
        {{3, 7}, {3, 5}}, {{2, 0}, {4, 2}}, {{2, 7}, {4, 5}},
        {{1, 0}, {2, 2}}, {{1, 7}, {2, 5}}, {{4, 0}, {2, 0}},
        {{4, 7}, {2, 7}}
    };
    EXPECT_EQ(game.MultiMoves(moves), move_success);
    EXPECT_EQ(game.IsOver(), false);
}

TEST(BasicIntegration, BasicCastleQueenSideUndo) {
    ChessGame game{};
    vector<MovePair> moves = {
        {{3, 1}, {3, 3}}, {{3, 6}, {3, 4}}, {{3, 0}, {3, 2}}, 
        {{3, 7}, {3, 5}}, {{2, 0}, {4, 2}}, {{2, 7}, {4, 5}},
        {{1, 0}, {2, 2}}, {{1, 7}, {2, 5}}, {{4, 0}, {2, 0}},
        {{4, 7}, {2, 7}}
    };
    EXPECT_EQ(game.MultiMoves(moves), move_success);

    game.UndoMove();
    game.UndoMove();
    EXPECT_EQ(game.Move({{4, 0}, {2, 0}}), move_success);
    EXPECT_EQ(game.Move({{4, 7}, {2, 7}}), move_success);
}