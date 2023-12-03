#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "game.hpp"
#include "raylib.h"
#include <map>

using std::vector;
using std::map;

const int pieces_amount = 12;
piece EMPTY_PIECE = piece{EMPTY_COLOR, EMPTY_TYPE};

class Graphics{
public:
    Graphics();
    void DrawGame(Game game);
    void SetMarkedSquare(square);
    
private:
    vector<Texture2D> piece_textures;
    vector<Texture2D> other_textures;
    //functions
    vector<Texture2D> LoadPieceTextures(vector<Image>& images);
    vector<Texture2D> LoadOtherTextures(vector<Image>& images);
    vector<Image> LoadPieceImages();
    vector<Image> LoadOtherImages();
    void ResizeImages(vector<Image>& images, int width, int height);
    int PieceIdx(int color, int type) {return type + 6 * color;};
    square marked = {-1, -1};
    inline bool MarkedSquare(){return marked.x > -1 && marked.y > -1;}

};

#endif 