#include "../include/game.hpp"

Game::Game(){
    //pawn = PieceImage();
}

Game::~Game(){
}

void Game::Draw(){
    DrawChessBoard();
    pawn.Draw();
}

void Game::Update(){
}
/*
void Game::StartScreen(){
    DrawText("Press SPACE to start", 200, 200, 20, WHITE);    
    while(!IsKeyDown(KEY_SPACE)){
        continue;
    }
}
*/

void Game::HandleInput(){
    if(IsKeyDown(KEY_RIGHT)){
        pawn.MoveRight();
    } else if(IsKeyDown(KEY_LEFT)){
        pawn.MoveLeft();
    } else if(IsKeyDown(KEY_UP)){
        pawn.MoveUp();
    } else if(IsKeyDown(KEY_DOWN)){
        pawn.MoveDown();
    }
}

void Game::DrawChessBoard() {
    const int boardSize = 8;
    const float squareSize = 100.0f; // Size of each square in pixels
    const float marginSize = 50.0f;  // Margin around the board in pixels

    for (int x = 0; x < boardSize; x++) {
        for (int y = 0; y < boardSize; y++) {
            Color color = ((x + y) % 2 == 0) ? DARKGRAY : RAYWHITE;
            DrawRectangle(x * squareSize + marginSize, y * squareSize + marginSize, squareSize, squareSize, color);
        }
    }
}





