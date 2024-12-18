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
        PieceColor player = PieceColor::W;
        InitWindow(windowWidth, windowHeight, "BetterChess BETA");
        SetTargetFPS(60);

        UI* ui = new UI;
        while(!WindowShouldClose()) {
            vector<vector<bool>> possibleMoves;
            ClearBackground(GRAY);
            BeginDrawing();
            ui->Draw(possibleMoves, player);
            EndDrawing();

            if(chessMatch && chessMatch->getCheckMate()){
                ui->DrawCheckMate(player, drawMenu);
            }
            
            EndDrawing();

            if(drawMenu){
                bool startNewGame = ui->DrawMenu(isGameBeingPlayed, drawMenu, player);
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
            try {
                player = chessMatch->getCurrentPlayer();
                ChessPosition source = ui->SelectPosition(possibleMoves, player, drawMenu);
                cout << source.toString() << endl;

                possibleMoves = chessMatch->possibleMoves(source);
                
                ChessPosition target = ui->SelectPosition(possibleMoves, player, drawMenu);

                std::cout << "destino escolhido" << std::endl;

                ChessPiece* capturedPiece = chessMatch->performChessMove(source, target);
                ui->MovePiece(source, target, chessMatch->getCastling());
                
                if (capturedPiece != nullptr){
                    ui->removePiece(capturedPiece);
                }

                if (chessMatch->getPromoted() != nullptr) {
                    ChessPiece* promoPiece = chessMatch->getPromoted();
                    ui->fillPromotionList(promoPiece->getColor());
                    string promotionType = ui->selectPromotionPiece(possibleMoves, player);
                    chessMatch->replacePromotedPiece(promotionType);
                    ui->replacePromotedPiece(promoPiece->getChessPosition(), promoPiece->getColor(), ui->toPieces(promotionType[0]));
                }
                chessMatch->nextTurn();
                isGameBeingPlayed = true;
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