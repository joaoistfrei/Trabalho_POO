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
        ChessMatch* chessMatch = new ChessMatch();
        int windowWidth = 1000;
        int windowHeight = 1000;
    
        // Color grey = {29, 29, 30, 255};

        InitWindow(windowWidth, windowHeight, "BetterChess BETA");
        SetTargetFPS(60);

        UI ui;
        while (!WindowShouldClose() || !chessMatch->getCheckMate()) {
            vector<vector<bool>> possibleMoves;
            ClearBackground(GRAY);
            BeginDrawing();
            ui.Draw(possibleMoves);
            EndDrawing();
            try {
                ChessPosition source = ui.SelectPosition(possibleMoves);
                cout << source.toString() << endl;

                possibleMoves = chessMatch->possibleMoves(source);
                
                ChessPosition target = ui.SelectPosition(possibleMoves);

                std::cout << "destino escolhido" << std::endl;

                ChessPiece* capturedPiece = chessMatch->performChessMove(source, target);
                ui.MovePiece(source, target, chessMatch->getCastling());
                
                if (capturedPiece != nullptr){
                    ui.removePiece(capturedPiece);
                }

                if (chessMatch->getPromoted() != nullptr) {
                    ChessPiece* promoPiece = chessMatch->getPromoted();
                    ui.fillPromotionList(promoPiece->getColor());
                    string promotionType = ui.selectPromotionPiece(possibleMoves);
                    chessMatch->replacePromotedPiece(promotionType);

                    ui.replacePromotedPiece(promoPiece->getChessPosition(), promoPiece->getColor(), ui.toPieces(promotionType[0]));
                }
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