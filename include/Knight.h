#ifndef KNIGHT_H
#define KNIGHT_H

#include "ChessPiece.h"
#include "Board.h"
#include "Position.h"
#include <vector>

class Knight : public ChessPiece {
private:
    bool canMove(Position position) const;

public:
    Knight(Board* board, PieceColor color);
    std::string toString() const override; // Adicionado o método toString
    std::vector<std::vector<bool>> possibleMoves() const override;
};

#endif // KNIGHT_H