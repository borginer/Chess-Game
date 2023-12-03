#include "graphics.hpp"

Graphics::Graphics(){
    vector<Image> piece_images = LoadPieceImages();
    this->piece_textures = LoadPieceTextures(piece_images);
    vector<Image> other_images = LoadOtherImages();
    this->other_textures = LoadOtherTextures(other_images);
}

void Graphics::DrawGame(Game game){
    int index; piece p;
    DrawTexture(other_textures[board_txt], sideBarSize, sideBarSize, WHITE);
    array<piece, 64> board = game.GetBoard();
    if(MarkedSquare()) 
        DrawRectangle(sideBarSize + marked.x * figureSize, 
                      sideBarSize + marked.y * figureSize, 
                      figureSize, figureSize, {220,0,0,255});
    for(int row = 0; row < 8; ++row){
        for(int col = 0; col < 8; ++col){
            index = square{col, row}.getIdx();
            p = board[index];
            if(p != EMPTY_PIECE){
                DrawTexture(piece_textures[PieceIdx(p.c, p.t)], 
                            sideBarSize + col * figureSize,
                            sideBarSize + row * figureSize, WHITE);
            }
        }
    }
}

vector<Texture2D> Graphics::LoadPieceTextures(vector<Image>& images){
    vector<Texture2D> textures(pieces_amount);
    for(int color = white; color != EMPTY_COLOR; ++color){
        for(int type = king; type != EMPTY_TYPE; ++type){
            textures[PieceIdx(color, type)] = 
                LoadTextureFromImage(images[(PieceIdx(color, type))]);
        }
    }
    return textures;
}

vector<Image> Graphics::LoadPieceImages(){
    vector<Image> images(pieces_amount);

    images[PieceIdx(white, king)]   = LoadImage("resources/wKing.png");
    images[PieceIdx(white, queen)]  = LoadImage("resources/wQueen.png");
    images[PieceIdx(white, rook)]   = LoadImage("resources/wRook.png");
    images[PieceIdx(white, bishop)] = LoadImage("resources/wBishop.png");
    images[PieceIdx(white, knight)] = LoadImage("resources/wKnight.png");
    images[PieceIdx(white, pawn)]   = LoadImage("resources/wPawn.png");

    images[PieceIdx(black, king)]   = LoadImage("resources/bKing.png");
    images[PieceIdx(black, queen)]  = LoadImage("resources/bQueen.png");
    images[PieceIdx(black, rook)]   = LoadImage("resources/bRook.png");
    images[PieceIdx(black, bishop)] = LoadImage("resources/bBishop.png");
    images[PieceIdx(black, knight)] = LoadImage("resources/bKnight.png");
    images[PieceIdx(black, pawn)]   = LoadImage("resources/bPawn.png");

    ResizeImages(images, figureSize, figureSize);
    return images;
}

vector<Texture2D> Graphics::LoadOtherTextures(vector<Image>& images){
    vector<Texture2D> textures;
    for(Image& img: images){
        textures.push_back(LoadTextureFromImage(img));
    }
    return textures;
}

vector<Image> Graphics::LoadOtherImages(){
    vector<Image> images(1);
    images[board_txt] = LoadImage("resources/board.png");
    ResizeImages(images, boardSize, boardSize);
    return images;
}

void Graphics::ResizeImages(vector<Image>& images, int width, int height){
    for(auto& image: images) {ImageResize(&image, width, height);}
}

void Graphics::SetMarkedSquare(square s){
    marked.x = s.x;
    marked.y = s.y;
}