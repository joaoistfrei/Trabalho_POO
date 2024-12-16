#include <raylib.h>
#include "../include/game.hpp"
#include <iostream>

int main() {
    int windowWidth = 1000;
    int windowHeight = 1000;
    bool isPieceSelected = false;

    Color grey = {29, 29, 30, 255};

    InitWindow(windowWidth, windowHeight, "BetterChess BETA");
    SetTargetFPS(60);

    Game game;

    while(!WindowShouldClose()){

        game.HandleInput(isPieceSelected);
        // std::cout << isPieceSelected << std::endl;
        game.Update();
        
        BeginDrawing();
        ClearBackground(grey);
        game.Draw();
        
        PieceImage teste(0, 0, {GetScreenHeight(), GetScreenWidth()});


        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}