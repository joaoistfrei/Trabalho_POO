#include "../include/graphics.hpp"
#include <iostream>

PieceImage::PieceImage(int piece, int color, Vector2 pos){
    Image img;
    switch (piece){
        case PAWN:{
            if(color)
                img = LoadImage("../Images/WhitePawn.png");
            else
                img = LoadImage("../Images/BlackPawn.png");            
        } break;
        case ROOK:{
            if(color)
                img = LoadImage("../Images/WhiteRook.png");
            else
                img = LoadImage("../Images/BlackRook.png");            
        } break;
        case KNIGHT:{
            if(color)
                img = LoadImage("../Images/WhiteKnight.png");
            else
                img = LoadImage("../Images/BlackKnight.png");            
        } break;
        case BISHOP:{
            if(color)
                img = LoadImage("../Images/WhiteBishop.png");
            else
                img = LoadImage("../Images/BlackBishop.png");            
        } break;
        case QUEEN:{
            if(color)
                img = LoadImage("../Images/WhiteQueen.png");
            else
                img = LoadImage("../Images/BlackQueen.png");            
        } break;
        case KING:{
            if(color)
                img = LoadImage("../Images/WhiteKing.png");
            else
                img = LoadImage("../Images/BlackKing.png");            
        } break;
        default:{
            img = LoadImage("../Images/Pawn.png"); 
        }break;
    }
    ImageResize(&img, 120, 200);
    image = LoadTextureFromImage(img);
    position.x = pos.x + (SQUARE_SIZE - image.width)/2;
    position.y = pos.y;
}

PieceImage::~PieceImage(){
    UnloadTexture(image);
}

void PieceImage::Draw(){
    DrawTextureV(image, position, WHITE);
    // DrawTextureEx(image, position, 0, 0.4, WHITE);
}


Vector2 PieceImage::getPosition(){
    Vector2 pos;
    pos.x = position.x;
    pos.y = position.y;
    return pos;
}

void PieceImage::MoveTo(Vector2 mousePos){
    position.x = mousePos.x*SQUARE_SIZE + (SQUARE_SIZE - image.width)/2;
    position.y = mousePos.y*SQUARE_SIZE;
}

// Queen::Queen(bool color){
//     Image img;
//     if(color)
//         img = LoadImage("../Images/WhiteQueen.png");
//     else
//         img = LoadImage("../Images/BlackQueen.png");

//     ImageResize(&img, 60, 100);
//     image = LoadTextureFromImage(img);
//     position.x = 300 + MARGIN_SIZE + (SQUARE_SIZE - img.width)/2;
//     position.y = 100 + MARGIN_SIZE;

//     // position.x = (GetScreenWidth() - image.width) / 2;
//     // position.y = (GetScreenHeight() - image.height
// }
