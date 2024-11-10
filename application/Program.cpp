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

using namespace std;

int main() {
    ChessMatch* chessMatch = new ChessMatch();
    vector<ChessPiece*> captured;

    while (!chessMatch->getCheckMate()) {
        try {
            UI::clearScreen();
            UI::printMatch(chessMatch, captured);
            cout << endl;
            cout << "Source: ";
            ChessPosition source = UI::readChessPosition();

            vector<vector<bool>> possibleMoves = chessMatch->possibleMoves(source);
            UI::clearScreen();
            UI::printBoard(chessMatch->getPieces(), possibleMoves);

            cout << endl;
            cout << "Target: ";
            ChessPosition target = UI::readChessPosition();

            ChessPiece* capturedPiece = chessMatch->performChessMove(source, target);

            if (capturedPiece != nullptr) {
                captured.push_back(capturedPiece);
            }

            if (chessMatch->getPromoted() != nullptr) {
                cout << "Enter piece for promotion (B/N/R/Q): ";
                string type;
                cin >> type;
                transform(type.begin(), type.end(), type.begin(), ::toupper);
                while (type != "B" && type != "N" && type != "R" && type != "Q") {
                    cout << "Invalid value! Enter piece for promotion (B/N/R/Q): ";
                    cin >> type;
                    transform(type.begin(), type.end(), type.begin(), ::toupper);
                }
                chessMatch->replacePromotedPiece(type);
            }

        } catch (const ChessException& e) {
            cout << e.what() << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } catch (const invalid_argument& e) {
            cout << e.what() << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    UI::clearScreen();
    UI::printMatch(chessMatch, captured);

    delete chessMatch;
    return 0;
}