#include <gtest/gtest.h>
#include <iostream>

#include "engine/game.hpp"

TEST(BasicIntegration, BasicMoveTest) {
    Game game{};
    EXPECT_EQ(game.Move({4, 1}, {4, 3}), move_success);
}

TEST(BasicIntegration, BasicCheckMate) {
    Game game{};
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
    Game game{};
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
    Game game{};
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
    Game game{};
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
    Game game{};
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
    Game game{};
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