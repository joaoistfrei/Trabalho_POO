#ifndef QUEEN_H
#define QUEEN_H

#include "ChessPiece.h"
#include "Board.h"
#include "Position.h"
#include <vector>

class Queen : public ChessPiece {
public:
    Queen(Board* board, Color color);
    std::string toString() const override; // Adicionado o método toString
    std::vector<std::vector<bool>> possibleMoves() const override;
};

#endif // QUEEN_H