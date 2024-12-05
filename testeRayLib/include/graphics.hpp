#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <raylib.h>

class PieceImage{
    public:
        PieceImage();
        ~PieceImage();
        void Draw();
        void MoveRight();
        void MoveLeft();
        void MoveUp();
        void MoveDown();
    
    private:
        Texture2D image;
        Vector2 position;
    
};

#endif