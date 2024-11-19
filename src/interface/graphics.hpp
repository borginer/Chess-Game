#pragma once

#include <map>
#include <vector>

#include "raylib.h"
#include "../engine/chess_game.hpp"

const int boardSize = 800;
const int sideBarSize = 20;
const int screenWidth = boardSize + 2 * sideBarSize;
const int screenHeight = boardSize + 2 * sideBarSize;
const int figureSize = boardSize / 8;

const int pieces_amount = 12;

class Graphics {
public:
    Graphics();
    void DrawGame(const ChessGame &game, PieceColor color);
    void SetMarkedSquare(Square);

private:
    vector<Texture2D> piece_textures;
    vector<Texture2D> other_textures;
    // functions
    vector<Texture2D> LoadPieceTextures(vector<Image> &images);
    vector<Texture2D> LoadOtherTextures(vector<Image> &images);
    vector<Image> LoadPieceImages();
    vector<Image> LoadOtherImages();
    void ResizeImages(vector<Image> &images, int width, int height);
    // calculate index in resources array
    inline int PieceIdx(int color, int type)
    {
        return type + 6 * color;
    };
    Square marked = {-1, -1};
    inline bool MarkedSquare()
    {
        return marked.x > -1 && marked.y > -1;
    }
};
