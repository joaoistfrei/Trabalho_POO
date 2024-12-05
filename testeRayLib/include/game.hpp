#ifndef GAME_HPP
#define GAME_HPP

#include <raylib.h>
#include "graphics.hpp"

class Game{
    public:
        Game();
        ~Game();
        void Draw();
        void Update();
        void HandleInput();
        //void StartScreen();
    private:
        PieceImage pawn;
        void DrawChessBoard();



};

#endif