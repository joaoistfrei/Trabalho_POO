#ifndef GAME_HPP
#define GAME_HPP

#include <raylib.h>
#include "graphics.hpp"
#include <vector>


class Game{
    public:
        Game();
        ~Game();
        void Draw();
        void Update();
        void HandleInput(bool& isPieceSelected);
        int GetSelectedPiece(Vector2 mousePos);
        //void StartScreen();

        const Vector2 invalidPos = {-1, -1};
    private:
        void DrawChessBoard();
        bool turn; // 0 = black, 1 = white
        std::vector<std::pair<int, PieceImage*>> _whitePieceList;
        std::vector<std::pair<int, PieceImage*>> _blackPieceList;
        int selectedPiece;
        PieceImage pawn;
};

#endif