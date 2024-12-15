#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <raylib.h>

#define MARGIN_SIZE 100.0f
#define SQUARE_SIZE 100.0f

enum Pieces{
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
};

class PieceImage{
    public:
        PieceImage(int piece = 0, int color = 0, Vector2 pos = {0,0});
        ~PieceImage();
        void Draw();
        void MoveRight();
        void MoveLeft();
        void MoveUp();
        void MoveDown();
        void MoveTo(Vector2 mousePos);
        Vector2 getPosition();
        int getImageWidth(){ return image.width; }
    
    protected:
        Texture2D image;
        Vector2 position;
    
};

#endif