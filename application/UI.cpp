#include "../include/UI.h"
#include "../include/ChessPiece.h"
#include "../include/ChessMatch.h"
#include "../include/ChessPosition.h"
#include "../include/Color.h"
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>

using namespace std;

const string ANSI_CYAN = "\033[36m";
const string ANSI_RESET = "\033[0m";
const string ANSI_BLUE_BACKGROUND = "\033[44m";
const string ANSI_WHITE = "\033[37m";
const string ANSI_YELLOW = "\033[33m";

void UI::clearScreen() {
    cout << "\033[2J\033[H";
}

void UI::printMatch(const ChessMatch* chessMatch, const vector<ChessPiece*>& captured) {
    printBoard(chessMatch->getPieces(), vector<vector<bool>>());
    cout << endl;
    printCapturedPieces(captured);
    cout << endl;
    cout << "Turn: " << chessMatch->getTurn() << endl;
    if (!chessMatch->getCheckMate()) {
        cout << "Waiting player: " << (chessMatch->getCurrentPlayer() == Color::WHITE ? "WHITE" : "BLACK") << endl;
        if (chessMatch->getCheck()) {
            cout << "CHECK!" << endl;
        }
    } else {
        cout << "CHECKMATE!" << endl;
        cout << "Winner: " << (chessMatch->getCurrentPlayer() == Color::WHITE ? "WHITE" : "BLACK") << endl;
    }
}

void UI::printBoard(const vector<vector<ChessPiece*>>& pieces, const vector<vector<bool>>& possibleMoves) {
    for (vector<ChessPiece*>::size_type i = 0; i < pieces.size(); i++) {
        cout << 8 - i << " ";
        for (vector<ChessPiece*>::size_type j = 0; j < pieces[i].size(); j++) {
            if (possibleMoves.size() > 0 && possibleMoves[i][j]) {
                cout << ANSI_BLUE_BACKGROUND;
            }
            printPiece(pieces[i][j], possibleMoves.size() > 0 && possibleMoves[i][j]);
            cout << ANSI_RESET << " ";
        }
        cout << endl;
    }
    cout << "  a b c d e f g h" << endl;
}

void printBoard(const vector<vector<ChessPiece*>>& pieces) {
    for (vector<ChessPiece*>::size_type i = 0; i < pieces.size(); i++) {
        cout << 8 - i << " ";
        for (vector<ChessPiece*>::size_type j = 0; j < pieces[i].size(); j++) {
            printPiece(pieces[i][j], false);
            cout << " ";
        }
        cout << endl;
    }
    cout << "  a b c d e f g h" << endl;
}

ChessPosition UI::readChessPosition() {
    string s;
    cin >> s;
    if (s.length() != 2) {
        throw invalid_argument("Error reading ChessPosition. Valid values are from a1 to h8.");
    }
    char column = s[0];
    int row = s[1] - '0';
    if (column < 'a' || column > 'h' || row < 1 || row > 8) {
        throw invalid_argument("Error reading ChessPosition. Valid values are from a1 to h8.");
    }
    //cout << ChessPosition(column, row).toString() << endl;
    return ChessPosition(column, row);
}

void printPiece(const ChessPiece* piece, bool background) {
    if (background) {
        cout << ANSI_BLUE_BACKGROUND;
    }
    if (piece == nullptr) {
        cout << "-" << ANSI_RESET;
    } else {
        if (piece->getColor() == Color::WHITE) {
            cout << ANSI_WHITE << piece->toString() << ANSI_RESET;
        } else {
            cout << ANSI_YELLOW << piece->toString() << ANSI_RESET;
        }
    }
}

void printCapturedPieces(const vector<ChessPiece*>& captured) {
    vector<ChessPiece*> white;
    vector<ChessPiece*> black;
    for (ChessPiece* piece : captured) {
        if (piece->getColor() == Color::WHITE) {
            white.push_back(piece);
        } else {
            black.push_back(piece);
        }
    }
    cout << "Captured pieces:" << endl;
    cout << "White: ";
    for (ChessPiece* piece : white) {
        cout << piece->toString() << " ";
    }
    cout << endl;
    cout << "Black: ";
    for (ChessPiece* piece : black) {
        cout << piece->toString() << " ";
    }
    cout << endl;
}