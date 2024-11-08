#include "graphics.hpp"

Piece EMPTY_PIECE = Piece{EMPTY_PIECE_COLOR, EMPTY_TYPE};

#define PATH_TO_ROOT "../"

Graphics::Graphics(){
    vector<Image> piece_images = LoadPieceImages();
    this->piece_textures = LoadPieceTextures(piece_images);
    vector<Image> other_images = LoadOtherImages();
    this->other_textures = LoadOtherTextures(other_images);
}

void Graphics::DrawGame(const Game& game) {
    int index; 
    Piece p;
    DrawTexture(other_textures[board_txt], sideBarSize, sideBarSize, WHITE);
    array<Piece, 64> board = game.GetBoard();
    if (MarkedSquare()) { 
        DrawRectangle(sideBarSize + marked.x * figureSize, 
                      sideBarSize + marked.y * figureSize, 
                      figureSize, figureSize, {220,0,0,255});
    }

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            index = Square{col, row}.getIdx();
            p = board[index];
            if (p != EMPTY_PIECE) {
                DrawTexture(piece_textures[PieceIdx(p.color, p.type)], 
                            sideBarSize + col * figureSize,
                            boardSize + sideBarSize - figureSize -
                             row * figureSize, WHITE);
            }
        }
    }
}

vector<Texture2D> Graphics::LoadPieceTextures(vector<Image>& images) {
    vector<Texture2D> textures(pieces_amount);
    for (int color = white; color != EMPTY_PIECE_COLOR; ++color) {
        for (int type = king; type != EMPTY_TYPE; ++type) {
            textures[PieceIdx(color, type)] = 
                LoadTextureFromImage(images[(PieceIdx(color, type))]);
        }
    }
    return textures;
}

vector<Texture2D> Graphics::LoadOtherTextures(vector<Image>& images) {
    vector<Texture2D> textures;
    for (Image& img: images) {
        textures.push_back(LoadTextureFromImage(img));
    }
    return textures;
}

vector<Image> Graphics::LoadPieceImages() {
    vector<Image> images(pieces_amount);

    images[PieceIdx(white, king)]   = LoadImage(PATH_TO_ROOT"resources/wKing.png");
    images[PieceIdx(white, queen)]  = LoadImage(PATH_TO_ROOT"resources/wQueen.png");
    images[PieceIdx(white, rook)]   = LoadImage(PATH_TO_ROOT"resources/wRook.png");
    images[PieceIdx(white, bishop)] = LoadImage(PATH_TO_ROOT"resources/wBishop.png");
    images[PieceIdx(white, knight)] = LoadImage(PATH_TO_ROOT"resources/wKnight.png");
    images[PieceIdx(white, pawn)]   = LoadImage(PATH_TO_ROOT"resources/wPawn.png");

    images[PieceIdx(black, king)]   = LoadImage(PATH_TO_ROOT"resources/bKing.png");
    images[PieceIdx(black, queen)]  = LoadImage(PATH_TO_ROOT"resources/bQueen.png");
    images[PieceIdx(black, rook)]   = LoadImage(PATH_TO_ROOT"resources/bRook.png");
    images[PieceIdx(black, bishop)] = LoadImage(PATH_TO_ROOT"resources/bBishop.png");
    images[PieceIdx(black, knight)] = LoadImage(PATH_TO_ROOT"resources/bKnight.png");
    images[PieceIdx(black, pawn)]   = LoadImage(PATH_TO_ROOT"resources/bPawn.png");

    ResizeImages(images, figureSize, figureSize);
    return images;
}

vector<Image> Graphics::LoadOtherImages() {
    vector<Image> images(1);
    images[board_txt] = LoadImage(PATH_TO_ROOT"resources/board.png");
    ResizeImages(images, boardSize, boardSize);
    return images;
}

void Graphics::ResizeImages(vector<Image>& images, int width, int height) {
    for(auto& image: images) {
        ImageResize(&image, width, height);
    }
}

void Graphics::SetMarkedSquare(Square s) {
    if (s.onBoard() || (s.x == -1 && s.y == -1)) {
        marked.x = s.x;
        marked.y = s.y;
    }
}