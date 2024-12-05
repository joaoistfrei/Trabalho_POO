#include <raylib.h>
#include "../include/game.hpp"

int main() {
    int windowWidth = 900;
    int windowHeight = 900;

    Color grey = {29, 29, 30, 255};

    InitWindow(windowWidth, windowHeight, "BetterChess BETA");
    SetTargetFPS(60);

    Game game;

    while(!WindowShouldClose()){

        game.HandleInput();
        game.Update();
        
        BeginDrawing();
        ClearBackground(grey);
        game.Draw();

        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}