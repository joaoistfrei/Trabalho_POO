#ifndef KING_H
#define KING_H

#include "ChessPiece.h"
#include "Board.h"
#include "Position.h"
#include <vector>

class ChessMatch;

class King : public ChessPiece {
private:
    ChessMatch* chessMatch;

    bool canMove(Position position) const;
    bool testRookCastling(Position position) const;

public:
    King(Board* board, PieceColor color, ChessMatch* chessMatch);
    std::string toString() const override; // Adicionado o método toString
    std::vector<std::vector<bool>> possibleMoves() const override;
};

#endif // KING_H