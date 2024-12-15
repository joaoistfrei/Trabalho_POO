#ifndef PAWN_H
#define PAWN_H

#include "ChessPiece.h"
#include "Board.h"
#include "Position.h"
#include <vector>

class ChessMatch;

class Pawn : public ChessPiece {
private:
    ChessMatch* chessMatch;

public:
    Pawn(Board* board, PieceColor color, ChessMatch* chessMatch);
    std::string toString() const override; // Adicionado o método toString
    std::vector<std::vector<bool>> possibleMoves() const override;
};

#endif // PAWN_H