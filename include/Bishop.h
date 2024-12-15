#ifndef BISHOP_H
#define BISHOP_H

#include "ChessPiece.h"
#include "Board.h"
#include "Position.h"
#include <vector>

class Bishop : public ChessPiece {
private:
    bool canMove(Position position) const;

public:
    Bishop(Board* board, PieceColor color);
    std::string toString() const override; // Adicionado o método toString
    std::vector<std::vector<bool>> possibleMoves() const override;
};

#endif // BISHOP_H