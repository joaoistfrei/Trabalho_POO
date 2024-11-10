#ifndef BISHOP_H
#define BISHOP_H

#include "ChessPiece.h"
#include "Board.h"
#include "Position.h"
#include <vector>

class Bishop : public ChessPiece {
public:
    Bishop(Board* board, Color color);
    std::string toString() const override; // Adicionado o método toString
    std::vector<std::vector<bool>> possibleMoves() const override;
};

#endif // BISHOP_H