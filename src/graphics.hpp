#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "game.hpp"
#include "raylib.h"
#include <map>
#include <vector>

using namespace std;

const int pieces_amount = 12;

class Graphics {
public:
    Graphics();
    void DrawGame(const Game &game);
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

#endif