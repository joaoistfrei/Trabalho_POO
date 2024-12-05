#include "../include/graphics.hpp"

PieceImage::PieceImage(){
    image = LoadTexture("Images/Pawn.png");
    position.x = (GetScreenWidth() - image.width) / 2;
    position.y = (GetScreenHeight() - image.height);
}

PieceImage::~PieceImage(){
    UnloadTexture(image);
}

void PieceImage::Draw(){
    DrawTextureV(image, position, WHITE);
}

void PieceImage::MoveRight(){
    position.x += 10;
    if(position.x > GetScreenWidth() - image.width){
        position.x = GetScreenWidth() - image.width;
    }
}

void PieceImage::MoveLeft(){
    position.x -= 10;
    if(position.x < 0){
        position.x = 0;
    }
}

void PieceImage::MoveUp(){
    position.y -= 10;
    if(position.y < 0){
        position.y = 0;
    }
}

void PieceImage::MoveDown(){
    position.y += 10;
    if(position.y > GetScreenHeight() - image.height){
        position.y = GetScreenHeight() - image.height;
    }
}