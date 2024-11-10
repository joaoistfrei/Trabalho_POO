#ifndef UI_H
#define UI_H

#include <iostream>
#include <vector>
#include <string>
#include "ChessMatch.h"
#include "ChessPiece.h"
#include "ChessPosition.h"

class UI {
public:
    static void clearScreen();
    static void printMatch(const ChessMatch* chessMatch, const std::vector<ChessPiece*>& captured);
    static void printBoard(const std::vector<std::vector<ChessPiece*>>& pieces, const std::vector<std::vector<bool>>& possibleMoves);
    static ChessPosition readChessPosition();
};

extern const std::string ANSI_CYAN;
extern const std::string ANSI_RESET;
extern const std::string ANSI_BLUE_BACKGROUND;
extern const std::string ANSI_WHITE;
extern const std::string ANSI_YELLOW;

void printPiece(const ChessPiece* piece, bool background);
void printCapturedPieces(const std::vector<ChessPiece*>& captured);

#endif // UI_H