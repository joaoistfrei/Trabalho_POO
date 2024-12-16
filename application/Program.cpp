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

        Color grey = {29, 29, 30, 255};

        InitWindow(windowWidth, windowHeight, "BetterChess BETA");
        SetTargetFPS(60);

        UI ui;
        while (!WindowShouldClose()) {
            vector<vector<bool>> possibleMoves;
            ClearBackground(grey);
            BeginDrawing();
            ui.Draw(possibleMoves);
            EndDrawing();
            try {
                ChessPosition source = ui.SelectPosition(possibleMoves);
                cout << source.toString() << endl;
                std::cout << "peça escolhida " << std::endl;
                possibleMoves = chessMatch->possibleMoves(source);
                
                ChessPosition target = ui.SelectPosition(possibleMoves);

                std::cout << "destino escolhido" << std::endl;

                ChessPiece* capturedPiece = chessMatch->performChessMove(source, target);
                ui.MovePiece(source, target);
                //cout << "CRAWLLLL" << endl;
                if (capturedPiece != nullptr){
                    ui.removePiece(capturedPiece);
                }

                // if (chessMatch->getPromoted() != nullptr) {
                //     cout << "Enter piece for promotion (B/N/R/Q): ";
                //     string type;
                //     cin >> type;
                //     transform(type.begin(), type.end(), type.begin(), ::toupper);
                //     while (type != "B" && type != "N" && type != "R" && type != "Q") {
                //         cout << "Invalid value! Enter piece for promotion (B/N/R/Q): ";
                //         cin >> type;
                //         transform(type.begin(), type.end(), type.begin(), ::toupper);
                //     }
                //     chessMatch->replacePromotedPiece(type);
                // }
            } catch (const ChessException& e) {
                cout << e.what() << endl;
                // cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } catch (const invalid_argument& e) {
                cout << e.what() << endl;
                // cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        CloseWindow();
        delete chessMatch;
        return 0;
    }