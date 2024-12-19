#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include "../include/Board.h"
#include "../include/BoardException.h"
#include "../include/ChessMatch.h"
#include "../include/ChessPiece.h"
#include "../include/ChessPosition.h"
#include "../include/Color.h"
#include "../include/Piece.h"
#include "../include/Position.h"
#include "../include/Rook.h"
#include "../include/Knight.h"
#include "../include/Bishop.h"
#include "../include/King.h"
#include "../include/Queen.h"
#include "../include/Pawn.h"
#include "../include/UI.h"
#include <limits>
#include <raylib.h>

using namespace std;

    int main() {
        ChessMatch* chessMatch = nullptr;
        int windowWidth = 1000;
        int windowHeight = 1000;
    
        bool drawMenu = true;
        bool isGameBeingPlayed = false;
        bool check = false;

        PieceColor player = PieceColor::W;
        InitWindow(windowWidth, windowHeight, "BetterChess BETA");
        SetTargetFPS(60);

        UI* ui = new UI;
        // Main game loop
        while(!WindowShouldClose()) {
            vector<vector<bool>> possibleMoves;
            ClearBackground(GRAY);
            BeginDrawing();
            // Game draw
            ui->Draw(possibleMoves, player, check);
            EndDrawing();
            // If there is a game and is checkmate, draw checkmate message
            if(chessMatch && chessMatch->getCheckMate()){
                ui->DrawCheckMate(player, drawMenu, check);
                if(WindowShouldClose())
                    break;
            }
            
            EndDrawing();
            // Try-catch to catch exceptions in case of irregular positions, windows closing, etc
            try {
                // Draw menu and reset game if necessary
                if(drawMenu){
                bool startNewGame = ui->DrawMenu(isGameBeingPlayed, drawMenu, player, check);
                
                if(startNewGame){
                    if(chessMatch != nullptr)
                        delete chessMatch;
                    if(isGameBeingPlayed){
                        delete ui;
                        ui = new UI;
                    }
                    chessMatch = new ChessMatch;
                    }
                    continue;
                }
                // Update check
                check = chessMatch->getCheck();
                // Get player's turn
                player = chessMatch->getCurrentPlayer();
                // Get position from UI
                ChessPosition source = ui->SelectPosition(possibleMoves, player, drawMenu, check);
                cout << source.toString() << endl;
                // Get piece possible moves
                possibleMoves = chessMatch->possibleMoves(source);
                // Get target movement position from UI
                ChessPosition target = ui->SelectPosition(possibleMoves, player, drawMenu, check);
                // Perform chosen move and get capturedPiece
                ChessPiece* capturedPiece = chessMatch->performChessMove(source, target);
                // Move piece on UI
                ui->MovePiece(source, target, chessMatch->getCastling());
                // Remove piece if there is a captured piece
                if (capturedPiece != nullptr){
                    ui->removePiece(capturedPiece, chessMatch->getEnPassantCompleted(), target);
                }
                // If there is a promotion, draw options, get selected piece and replace in game and UI
                if (chessMatch->getPromoted() != nullptr) {
                    ChessPiece* promoPiece = chessMatch->getPromoted();
                    ui->fillPromotionList(promoPiece->getColor());
                    string promotionType = ui->selectPromotionPiece(possibleMoves, player, check);
                    chessMatch->replacePromotedPiece(promotionType);
                    ui->replacePromotedPiece(promoPiece->getChessPosition(), promoPiece->getColor(), ui->toPieces(promotionType[0]));
                }
                // Set next turn
                chessMatch->nextTurn();
                isGameBeingPlayed = true;
            // Catch 
            } catch (const ChessException& e) {
                cout << e.what() << endl;
            } catch (const invalid_argument& e) {
                cout << e.what() << endl;
            } catch (const runtime_error & e) {
                cout << e.what() << endl;
                break;
            }
        }
        CloseWindow();
        delete chessMatch;
        return 0;
    }