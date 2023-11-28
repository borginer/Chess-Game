#include "raylib.h"
#include <vector>
#include "game.hpp"

using std::vector;
void LoadImages(vector<Image>&);
void LoadTextures(vector<Image>&, vector<Texture2D>&);
void ResizeImages(vector<Image>&);

const int image_amount = OTHER_LAST;

int main(){
    InitWindow(screenWidth, screenHeight, "Chess Simulator");
    vector<Image> images(image_amount);
    vector<Texture2D> textures(image_amount);

    LoadImages(images);
    ResizeImages(images);
    LoadTextures(images, textures);
    Game g(textures);

    bool piece_pressed = false;
    square from;
    int x, y;

    SetTargetFPS(60);
    // Main Game Loop
    while(!WindowShouldClose()){
        BeginDrawing();

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            Vector2 pos = GetMousePosition();
            x = std::floor((pos.x - sideBarSize) / figureSize);
            y = std::floor((pos.y - sideBarSize) / figureSize);
            if(piece_pressed){
                piece_pressed = false;
                g.SetMarkedSquare({});
                g.Move(from, {x, y});
            }
            else{
                piece_pressed = true;
                g.SetMarkedSquare({x, y});
                from = {x, y};
            }
        }
        ClearBackground({103, 43, 0});
        g.DrawBoard();

        EndDrawing();
    }
    for(auto texture: textures) UnloadTexture(texture);
    CloseWindow();
}

void LoadImages(vector<Image>& images){
    images[wKing] = LoadImage("resources/wKing.png");
    images[wQueen] = LoadImage("resources/wQueen.png");
    images[wRook] = LoadImage("resources/wRook.png");
    images[wBishop] = LoadImage("resources/wBishop.png");
    images[wKnight] = LoadImage("resources/wKnight.png");
    images[wPawn] = LoadImage("resources/wPawn.png");

    images[bKing] = LoadImage("resources/bKing.png");
    images[bQueen] = LoadImage("resources/bQueen.png");
    images[bRook] = LoadImage("resources/bRook.png");
    images[bBishop] = LoadImage("resources/bBishop.png");
    images[bKnight] = LoadImage("resources/bKnight.png");
    images[bPawn] = LoadImage("resources/bPawn.png");
    
    images[board_txt] = LoadImage("resources/board.png");
}

void LoadTextures(vector<Image>& images, vector<Texture2D>& textures){
    for(int x = wKing; x != TYPES_LAST; ++x){
        textures[x] = LoadTextureFromImage(images[x]);
    }
    for(int x = board_txt; x != OTHER_LAST; ++x){
        textures[x] = LoadTextureFromImage(images[x]);
    }
}

void ResizeImages(vector<Image>& images){
    ImageResize(&images[board_txt], boardSize,boardSize);
    for(int x = wKing; x != board_txt; ++x){
        ImageResize(&images[x], figureSize, figureSize);
    }
}
